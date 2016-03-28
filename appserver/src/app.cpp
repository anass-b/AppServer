//
//  App.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <app.h>
#include <server.h>
#include <sdlwindow.h>
#include <sdlcompositor.h>
#include <geometry.h>
#include <protocol.h>

#include <sstream>
#include <string.h>
#include <pthread.h>
#include <zlib.h>
#include <minilzo/minilzo.h>

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
    int socketPort = AspReqListenerThreadPortValue + _id;
    socketAddress << socketPort;
    _socket->bind(socketAddress.str());

    // Event socket
    _eventSocket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REQ);
    std::stringstream eventSocketAddress;
    eventSocketAddress << "tcp://";
    eventSocketAddress << Server::getSingleton()->getAppsHost();
    eventSocketAddress << ":";
    int eventSocketPort = 10000 + _id;
    eventSocketAddress << eventSocketPort;
    _eventSocket->connect(eventSocketAddress.str());

    if (pthread_create(&_requestListener, NULL, App::requestListener, this)) {
        throw std::runtime_error(strerror(errno));
    }
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
        app->printException(e);
    }

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
        else if (req.type == AspRequestUnregister) {
            _runRequestListener = false;
        }
    } catch (std::exception e) {
        this->printException(e);
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
    try {
        std::shared_ptr<zmq::socket_t> socket = this->getSocket().lock();
        
        sendAck(socket);

        unsigned char *data = (unsigned char*)malloc(req.dataSize);
        size_t receivedSize = socket->recv(data, req.dataSize);
        if (receivedSize <= 0) return;
        
        TWindowId id = Window::newWindowId();
        
        Asp_Event evt;
        evt.winId = id;
        zmq::message_t response(&evt, sizeof(Asp_Event));        
        socket->send(response);

        Rect windowFrame = makeRect(static_cast<double>(req.field0),
                                    static_cast<double>(req.field1),
                                    static_cast<double>(req.field2),
                                    static_cast<double>(req.field3));
        
        std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
        std::shared_ptr<SDLCompositor> sdlCompositor = std::dynamic_pointer_cast<SDLCompositor>(compositor);
        
        std::shared_ptr<SDLWindow> newWindow = std::make_shared<SDLWindow>(shared_from_this(), id, windowFrame, (int)req.field4);
        newWindow->setRenderer(sdlCompositor->getRenderer());
        newWindow->create(data, req.dataSize);
        
        compositor->addWindow(std::move(newWindow));
    }
    catch (std::exception e) {        
        this->printException(e);
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
        return;
    }
    
    try {        
        std::shared_ptr<zmq::socket_t> socket = this->getSocket().lock();
        
        sendAck(socket);

        bool compression = req.field4;
        unsigned char* data = nullptr;

        uint64_t receivedDataSize = compression ? req.compressedSize : req.dataSize;
        unsigned char *receivedData = new unsigned char[receivedDataSize];
        size_t receivedSize = socket->recv(receivedData, receivedDataSize);
        if (receivedSize <= 0) return;

        if (compression) {
            // Decompress received data
            data = new unsigned char[req.dataSize];
            lzo_uint ucompSize = req.dataSize;
            int r = lzo1x_decompress(receivedData, req.compressedSize, data, &ucompSize, NULL);
            if (r != LZO_E_OK || ucompSize != req.dataSize) {
                return;
            }
            delete[] receivedData;
        }
        else {
            data = receivedData;
        }
        
        sendAck(socket);
        
        window->updatePixels(data, req.dataSize, makeRect(req.field0, req.field1, req.field2, req.field3));
    } catch (std::exception e) {
        this->printException(e);
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
        return;
    }
    
    try {
        std::shared_ptr<zmq::socket_t> socket = this->getSocket().lock();
        
        sendAck(socket);

        unsigned char *data = (unsigned char*)malloc(req.dataSize);
        size_t receivedSize = socket->recv(data, req.dataSize);
        if (receivedSize <= 0) return;
        
        sendAck(socket);
        
        window->setSize(makeSize(req.field0, req.field1));
        window->resize(data, req.dataSize);
    } catch (std::exception e) {
        this->printException(e);
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
        return;
    }
    
    window->setVisible(req.field0);
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
        return;
    }
    
    Point newLocation = makePoint(req.field0, req.field1);
    window->setLocation(newLocation);
    window->move(newLocation);
}

void App::bringWindowToFront(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        return;
    }
    
    Server::getSingleton()->getCompositor().lock()->bringWindowToFront(window);
}

void App::destroyWindow(Asp_Request req)
{
    TWindowId windowId = req.winId;
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(windowId);
    if (window == nullptr) {
        return;
    }
    
    Server::getSingleton()->getCompositor().lock()->removeWindow(windowId);
}

void App::sendMouseMoveEvent(std::shared_ptr<MouseMoveEvent> evt, std::shared_ptr<Window> window)
{
    try {                
        Asp_Event req = evt->toProtocolEvent();

        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
            
        recvAck(_eventSocket);
    }
    catch (zmq::error_t e) {
        this->printException(e);
    }
}

void App::sendMouseButtonEvent(std::shared_ptr<MouseButtonEvent> evt, std::shared_ptr<Window> window)
{
    try {        
        Asp_Event req = evt->toProtocolEvent();
        
        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
        
        recvAck(_eventSocket);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

void App::sendMouseScrollEvent(std::shared_ptr<MouseScrollEvent> evt, std::shared_ptr<Window> window)
{
    try {        
        Asp_Event req = evt->toProtocolEvent();

        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);

        recvAck(_eventSocket);
    }
    catch (std::exception e) {
       this->printException(e);
    }
}

void App::sendTextEvent(std::shared_ptr<TextEvent> evt, std::shared_ptr<Window> window)
{
    try {
        Asp_Event req = evt->toProtocolEvent();
        
        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
        
        recvAck(_eventSocket);
        
        zmq::message_t textRequest(evt->getText().c_str(), evt->getText().size());
        _eventSocket->send(textRequest);
        
        recvAck(_eventSocket);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

void App::sendKeyEvent(std::shared_ptr<KeyEvent> evt, std::shared_ptr<Window> window)
{
    try {
        Asp_Event req = evt->toProtocolEvent();
        
        zmq::message_t eventRequest(&req, sizeof(Asp_Event));
        _eventSocket->send(eventRequest);
        
        recvAck(_eventSocket);
    }
    catch (std::exception e) {
        this->printException(e);
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

void App::printException(const std::exception &e) const
{
    std::cout << e.what() << std::endl;
}

bool App::sendAck(std::shared_ptr<zmq::socket_t> socket)
{
    int ack = 1;
    zmq::message_t msg(&ack, sizeof(uint8_t));
    return socket->send(msg);
}

bool App::recvAck(std::shared_ptr<zmq::socket_t> socket)
{
    int ack = 0;
    size_t receivedSize = socket->recv(&ack, sizeof(uint8_t));
    if (receivedSize <= 0 || ack != 1) {
        return false;
    }
    return true;
}
