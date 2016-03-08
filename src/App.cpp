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
#include <Asl/Logger.h>
#include <Asl/Protocol.h>
#include <Asl/Connector.h>

#include <sstream>
#include <string.h>
#include <pthread.h>

using namespace appserver;
using namespace asl;

unsigned long App::_counter = 1;

App::App(TProcId pid) : _id(++_counter), _pid(pid), _busy(false)
{
    _msgq = std::make_shared<message_queue>(open_only, Connector::messageQueueNameFromPid(pid).c_str());
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
    } catch (exception e) {
        Logger::log(e.what(), __func__);
        string what(e.what());
        Logger::log("An exception occured while excecuting request: " + what);
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
        string msg = "Invalid window width given by client app";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    if (req.field4 < 0 || req.field4 > Window::kMaxHeight) {
        string msg = "Invalid window height given by client app";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    if (req.dataSize > Window::kMaxDataSize) {
        string msg = "Invalid raster data size given by client app";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    if (req.field5 != AspWindowRasterARGB && req.field5 != AspWindowRasterRGBA) {
        string msg = "Invalid raster tyoe given by client app";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    
    try {
        int remoteSock = asl::Connector::connect(asl::Connector::sockPathFromPid(_pid).c_str());
        if (remoteSock == 0) {
            perror("asl::Connector::connect");
        }
        unsigned char *data = (unsigned char*)malloc(req.dataSize);
        asl::Connector::receiveAll(remoteSock, data, req.dataSize, MSG_WAITALL);
        asl::Connector::closeSocket(remoteSock);
        
        TWindowId id = Window::newWindowId();
        
        if (Logger::isEnabled()) {
            ostringstream msg;
            msg << "Window ID=" << id << " created";
            Logger::log(__func__, msg.str());
        }
        
        Asp_Event evt;
        evt.winId = id;
        _msgq->send(&evt, sizeof(evt), 0);

        Rect windowFrame = makeRect(static_cast<double>(req.field0),
                                    static_cast<double>(req.field1),
                                    static_cast<double>(req.field2),
                                    static_cast<double>(req.field3));
        
        std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
        
        std::shared_ptr<Window> newWindow;
        if (Server::getSingleton()->getBackendMode() == kBackendModeSDL) {
            newWindow = make_shared<SDLWindow>(shared_from_this(), id, windowFrame, (int)req.field5, (bool)req.field4);
            
            std::shared_ptr<SDLCompositor> sdlCompositor = std::dynamic_pointer_cast<SDLCompositor>(compositor);
            std::shared_ptr<SDLWindow> sdlWindow = std::dynamic_pointer_cast<SDLWindow>(newWindow);
            sdlWindow->setRenderer(sdlCompositor->getRenderer());
        }
        else if (Server::getSingleton()->getBackendMode() == kBackendModeGLFW) {
            newWindow = make_shared<GLWindow>(shared_from_this(), id, windowFrame, (int)req.field5, (bool)req.field4);
        }
        
        
        newWindow->create(data, req.dataSize);
        compositor->addWindow(std::move(newWindow));
    }
    catch (std::exception e) {
        Logger::log(e.what(), __func__);
        throw std::runtime_error(__func__);
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
        string msg = "Invalide Window ID given by client app";
        Logger::log(msg, __func__);
        throw std::invalid_argument("Invalide window ID");
    }
    
    try {
        int remoteSock = asl::Connector::connect(asl::Connector::sockPathFromPid(_pid).c_str());
        void *data = malloc(req.dataSize);
        asl::Connector::receiveAll(remoteSock, data, req.dataSize, MSG_WAITALL);
        asl::Connector::closeSocket(remoteSock);
        
        window->updatePixels(data, req.dataSize, makeRect(req.field1, req.field2, req.field3, req.field4));
    } catch (exception e) {
        Logger::log(e.what(), __func__);
        throw runtime_error(__func__);
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
        string msg = "Invalide window ID given client app";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    
    try {
        int remoteSock = asl::Connector::connect(asl::Connector::sockPathFromPid(_pid).c_str());
        void *data = malloc(req.dataSize);
        asl::Connector::receiveAll(remoteSock, data, req.dataSize, MSG_WAITALL);
        asl::Connector::closeSocket(remoteSock);
        
        window->setSize(makeSize(req.field1, req.field2));
        window->resize(data, req.dataSize);
    } catch (exception e) {
        Logger::log(e.what(), __func__);
        throw runtime_error(__func__);
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
        string msg = "Invalide window ID";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
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
        string msg = "Invalide window ID";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    
    Point newLocation = makePoint(req.field1, req.field2);
    window->setLocation(newLocation);
    window->move(newLocation);
}

void App::bringWindowToFront(Asp_Request req)
{
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(req.winId);
    if (window == nullptr) {
        string msg = "Invalide window ID";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    
    Server::getSingleton()->getCompositor().lock()->bringWindowToFront(window);
}

void App::destroyWindow(Asp_Request req)
{
    TWindowId windowId = req.winId;
    std::shared_ptr<Window> window = Server::getSingleton()->getCompositor().lock()->findWindow(windowId);
    if (window == nullptr) {
        string msg = "Invalide window ID";
        Logger::log(msg, __func__);
        throw std::invalid_argument(msg);
    }
    
    Server::getSingleton()->getCompositor().lock()->removeWindow(windowId);
}

void App::sendMouseMoveEvent(TWindowId windowId, int type, double x, double y, double absX, double absY)
{
    try {
        if (type == -1) {
            Asp_Event req;
            req.winId = windowId;
            req.type = AspEventWindowLocationChanged;
            req.field0 = x;
            req.field1 = y;
            
            _msgq->send(&req, sizeof(req), 0);
        }
        else {
            Asp_Event req;
            req.winId = windowId;
            req.type = AspEventMouseInput;
            req.field0 = x;
            req.field1 = y;
            req.field2 = absX;
            req.field3 = absY;
            req.field4 = type;
            
            _msgq->send(&req, sizeof(req), 0);
        }
        
    } catch (exception e) {
        Logger::log(e.what(), __func__);
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
        
        _msgq->send(&req, sizeof(req), 0);
    } catch (exception e) {
        Logger::log(e.what(), __func__);
    }
}

void App::sendKeyEvent(TWindowId windowId, int charCode)
{
    try {
        Asp_Event req;
        req.winId = windowId;
        req.type = AspEventKeyInput;
        req.field0 = charCode;

        _msgq->send(&req, sizeof(req), 0);
    } catch (exception e) {
        Logger::log(e.what(), __func__);
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

std::weak_ptr<message_queue> App::getMessageQueue() const
{
    return _msgq;
}

App::~App()
{
    Server::getSingleton()->getCompositor().lock()->removeWindows(_id);
    message_queue::remove(Connector::messageQueueNameFromPid(_pid).c_str());
}


