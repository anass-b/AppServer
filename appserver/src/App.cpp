//
//  App.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <App.h>
#include <Server.h>
#include <GLWindow.h>
#include <SDLWindow.h>
#include <SDLCompositor.h>
#include <Geometry.h>
#include <protocol.h>

#include <sstream>
#include <string.h>
#include <pthread.h>

using namespace appserver;

unsigned long App::_counter = 1;

App::App(TProcId pid) : _id(++_counter), _pid(pid), _busy(false)
{    
}

void App::startRequestListener()
{
    _context = std::make_shared<zmq::context_t>(1);

    // Requests socket
    _socket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REP);
    std::stringstream socketAddress;
    socketAddress << "tcp://*:";
    int socketPort = 20000 + _id;
    socketAddress << socketPort;
    _socket->bind(socketAddress.str());
    std::cout << "Started requests socket in port " << socketPort << std::endl;

    // Event socket
    _eventSocket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REQ);
    std::stringstream eventSocketAddress;
    eventSocketAddress << "tcp://";
    eventSocketAddress << Server::getSingleton()->getAppsHost();
    eventSocketAddress << ":";
    int eventSocketPort = 10000 + _id;
    eventSocketAddress << eventSocketPort;
    _eventSocket->connect(eventSocketAddress.str());
    std::cout << "Connected to events socket in port " << eventSocketPort << std::endl;

    if (pthread_create(&_requestListener, NULL, App::requestListener, this)) {
        throw std::runtime_error(strerror(errno));
    }
}

void App::stopRequestListener()
{
    _runRequestListener = false;
    _context->close();
}

void* App::requestListener(void* arg)
{
    App* app = (App*)arg;

    try {
        while (app->_runRequestListener) {
            Asp_Request req;
            size_t receivedSize = app->getSocket().lock()->recv(&req, sizeof(Asp_Request));
            if (receivedSize > 0) {
                app->processMessage(req);
            }
        }
    }
    catch (zmq::error_t e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
    
    std::cout << "Request thread terminated." << std::endl;

    return nullptr;
}

void App::processMessage(Asp_Request req)
{
    try {
        if (req.type == AspRequestCreateWindow) {
            newWindow(req);
        }
        else if (req.type == AspRequestMoveWindow) {
            moveWindow(req);
        }
        else if (req.type == AspRequestSetWindowVisibility) {
            setWindowVisibility(req);
        }
        else if (req.type == AspRequestBringWindowToFront) {
            bringWindowToFront(req);
        }
        else if (req.type == AspRequestUpdateWindowSurface) {
            updateWindow(req);
        }
        else if (req.type == AspRequestResizeWindow) {
            resizeWindow(req);
        }
        else if (req.type == AspRequestDestroyWindow) {
            destroyWindow(req);
        }
    } catch (std::exception e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}


/*
 * Create a window
 *
 * field0   : X
 * field1   : Y
 * field2   : Width
 * field3   : Height
 * field4   : Visibility
 */
void App::newWindow(Asp_Request req)
{
    if (req.field3 < 0 || req.field3 > Window::kMaxWidth) {
        std::cout << __func__ << ": " << "Invalid window width given by client app" << std::endl;
    }
    if (req.field4 < 0 || req.field4 > Window::kMaxHeight) {
        std::cout << __func__ << ": " << "Invalid window height given by client app" << std::endl;
    }
    if (req.dataSize > Window::kMaxDataSize) {
        std::cout << __func__ << ": " << "Invalid raster data size given by client app" << std::endl;
    }
    if (req.field5 != AspWindowRasterARGB && req.field5 != AspWindowRasterRGBA) {
        std::cout << __func__ << ": " << "Invalid raster tyoe given by client app" << std::endl;
    }
    
    try {
        std::shared_ptr<zmq::socket_t> socket = this->getSocket().lock();
        
        // ACK
        int ack = 1;
        zmq::message_t ackResponse(&ack, sizeof(int));
        socket->send(ackResponse);

        unsigned char *data = (unsigned char*)malloc(req.dataSize);
        size_t receivedSize = socket->recv(data, req.dataSize);
        if (receivedSize <= 0) return;
        
        TWindowId id = Window::newWindowId();
        
        std::ostringstream msg;
        msg << "Window ID=" << id << " created";
        std::cout << __func__ << ": " << msg.str() << std::endl;
        
        Asp_Event evt;
        evt.winId = id;
        zmq::message_t response(&evt, sizeof(Asp_Event));        
        socket->send(response);

        Rect windowFrame = makeRect(static_cast<double>(req.field0),
                                    static_cast<double>(req.field1),
                                    static_cast<double>(req.field2),
                                    static_cast<double>(req.field3));
        
        std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
        
        std::shared_ptr<Window> newWindow;
        if (Server::getSingleton()->getBackendMode() == kBackendModeSDL) {
            newWindow = std::make_shared<SDLWindow>(shared_from_this(), id, windowFrame, (int)req.field5, (bool)req.field4);
            
            std::shared_ptr<SDLCompositor> sdlCompositor = std::dynamic_pointer_cast<SDLCompositor>(compositor);
            std::shared_ptr<SDLWindow> sdlWindow = std::dynamic_pointer_cast<SDLWindow>(newWindow);
            sdlWindow->setRenderer(sdlCompositor->getRenderer());
        }
        else if (Server::getSingleton()->getBackendMode() == kBackendModeGLFW) {
            newWindow = std::make_shared<GLWindow>(shared_from_this(), id, windowFrame, (int)req.field5, (bool)req.field4);
        }
        
        
        newWindow->create(data, req.dataSize);
        compositor->addWindow(std::move(newWindow));
    }
    catch (std::exception e) {        
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}

/*
 * Update a window's raster
 *
 * field1   : X
 * field2   : Y
 * field3   : Width
 * field4   : Height
 */
void App::updateWindow(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        std::cout << __func__ << ": " << "Invalide Window ID given by client app" << std::endl;
    }
    
    try {        
        std::shared_ptr<zmq::socket_t> socket = this->getSocket().lock();
        
        // ACK
        int ack = 1;
        zmq::message_t ackResponse1(&ack, sizeof(int));
        socket->send(ackResponse1);

        unsigned char *data = (unsigned char*)malloc(req.dataSize);
        size_t receivedSize = socket->recv(data, req.dataSize);
        if (receivedSize <= 0) return;
        
        // ACK
        ack = 1;
        zmq::message_t ackResponse2(&ack, sizeof(int));
        socket->send(ackResponse2);
        
        window->updatePixels(data, req.dataSize, makeRect(req.field1, req.field2, req.field3, req.field4));
    } catch (std::exception e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}

/*
 * Resize a window
 *
 * field1   : Width
 * field2   : Height
 */
void App::resizeWindow(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        std::cout << __func__ << ": " << "Invalide window ID given client app" << std::endl;
    }
    
    try {
        std::shared_ptr<zmq::socket_t> socket = this->getSocket().lock();
        
        // ACK
        int ack = 1;
        zmq::message_t ackResponse1(&ack, sizeof(int));
        socket->send(ackResponse1);

        unsigned char *data = (unsigned char*)malloc(req.dataSize);
        size_t receivedSize = socket->recv(data, req.dataSize);
        if (receivedSize <= 0) return;
        
        // ACK
        ack = 1;
        zmq::message_t ackResponse2(&ack, sizeof(int));
        socket->send(ackResponse2);
        
        window->setSize(makeSize(req.field1, req.field2));
        window->resize(data, req.dataSize);
    } catch (std::exception e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}

/*
 * Change a window's visibility
 *
 * field1 : Visiblility
 */
void App::setWindowVisibility(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        std::cout << __func__ << ": " << "Invalide window ID" << std::endl;
    }
    
    window->setVisible(req.field1);
}

/*
 * Change a window's location
 *
 * field1 : X
 * field2 : Y
 */
void App::moveWindow(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        std::cout << __func__ << ": " << "Invalide window ID" << std::endl;
    }
    
    Point newLocation = makePoint(req.field1, req.field2);
    window->setLocation(newLocation);
    window->move(newLocation);
}

void App::bringWindowToFront(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        std::cout << __func__ << ": " << "Invalide window ID" << std::endl;
    }
    
    Server::getSingleton()->getCompositor().lock()->bringWindowToFront(window);
}

void App::destroyWindow(Asp_Request req)
{
    TWindowId windowId = req.winId;
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(windowId);
    if (window == nullptr) {
        std::cout << __func__ << ": " << "Invalide window ID" << std::endl;
    }
    
    Server::getSingleton()->getCompositor().lock()->removeWindow(windowId);
}

void App::sendMouseMoveEvent(TWindowId windowId, int type, double x, double y, double absX, double absY)
{
    try {
        Asp_Event req;
        if (type == -1) {            
            req.winId = windowId;
            req.type = AspEventWindowLocationChanged;
            req.field0 = x;
            req.field1 = y;                        
        }
        else {
            req.winId = windowId;
            req.type = AspEventMouseInput;
            req.field0 = x;
            req.field1 = y;
            req.field2 = absX;
            req.field3 = absY;
            req.field4 = type;                       
        }
        
        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
            
        int ack = 0;
        size_t receivedSize = _eventSocket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
    } catch (zmq::error_t e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}

void App::sendMouseButtonEvent(TWindowId windowId, int type, int button, double x, double y, double absX, double absY)
{
    try {
        Asp_Event req;
        req.winId = windowId;
        req.type = AspEventMouseInput;
        req.field0 = x;
        req.field1 = y;
        req.field2 = absX;
        req.field3 = absY;
        req.field4 = type;
        req.field5 = button;
        
        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
        
        int ack = 0;
        size_t receivedSize = _eventSocket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
    } catch (std::exception e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}

void App::sendKeyEvent(TWindowId windowId, int charCode)
{
    try {
        Asp_Event req;
        req.winId = windowId;
        req.type = AspEventKeyInput;
        req.field0 = charCode;
        
        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
        
        int ack = 0;
        size_t receivedSize = _eventSocket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
    } catch (std::exception e) {
        std::cout << __func__ << ": " << e.what() << std::endl;
    }
}

TProcId App::getPid() const
{
    return _pid;
}

TAppId App::getId() const
{
    return _id;
}

std::weak_ptr<zmq::socket_t> App::getSocket() const
{
    return _socket;
}


std::weak_ptr<zmq::socket_t> App::getEventSocket() const
{
    return _eventSocket;
}

App::~App()
{
    Server::getSingleton()->getCompositor().lock()->removeWindows(_id);
}


