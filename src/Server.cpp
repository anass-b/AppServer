//
//  server.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <Server.h>
#include <SDLWorkspace.h>
#include <SDLCompositor.h>
#include <GLCompositor.h>
#include <GlfwWorkspace.h>
#include <Asl/Logger.h>
#include <Asl/Connector.h>
#include <Asl/Utils.h>

#include <pthread.h>
#include <signal.h>

using namespace appserver;

Server* Server::_sharedInst = nullptr;

Server::Server()
{
    _context = std::make_shared<zmq::context_t>(1);
    _socket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REP);
    _socket->bind("tcp://*:5555");
}

void Server::dispatchMessage(Asp_Request req)
{
    if (req.type == AspRequestRegister) {
        std::shared_ptr<App> app = nullptr;
        app = make_shared<App>(req.field0);
        
        // Send the client ID back to the app
        Asp_Event evt;
        evt.field0 = app->getId();
        std::shared_ptr<zmq::socket_t> socket = Server::getSingleton()->getSocket().lock();
        zmq::message_t response(&evt, sizeof(Asp_Event));
        socket->send(response);
        
        app->createAndConnectSocket();

        addApp(app);
    }
    else {
        std::shared_ptr<App> app = findApp(req.clientId).lock();
        if (app != nullptr) {
            if (req.type == AspRequestUnregister) {
                removeAppById(app->getId());                
            }
            else {
                app->processMessage(req);
            }
        }
        else {
            asl::Logger::log("Requested app not found");
        }
    }
}

void* Server::requestListener(void *ptr)
{
    Server* server = Server::getSingleton();
    std::shared_ptr<zmq::socket_t> socket = server->getSocket().lock();
    for (;;) {
        try {
            Asp_Request req;        
            size_t receivedSize = socket->recv(&req, sizeof(Asp_Request));
            if (receivedSize > 0) {
                server->dispatchMessage(req);
            }
        }
        catch (zmq::error_t e) {
            asl::Logger::log(e.what(), __func__);
        }
    }
    return nullptr;
}

void* Server::processMonitor(void *ptr)
{
    Server* server = Server::getSingleton();
    
    while (true) {
        asl::avoidBusyWait(10 * NANO_SECOND_MULTIPLIER);
        std::vector<std::shared_ptr<App>> apps = server->_apps;
        for (int i = 0; i < apps.size(); i++) {
            std::shared_ptr<App> app = apps.at(i);
            if (kill(app->getPid(), 0) == -1) {
                server->removeAppById(app->getId());
            }
        }
    }
    return nullptr;
}

void Server::run(BackendMode backendMode)
{
    _backendMode = backendMode;
    
    if (pthread_create(&_messageDispatcher, NULL, Server::requestListener, NULL)) {
        throw runtime_error(strerror(errno));
    }
    
    if (pthread_create(&_processMonitor, NULL, Server::processMonitor, NULL)) {
        throw runtime_error(strerror(errno));
    }
    
    _windowManager = std::make_shared<WindowManager>();
    
    std::shared_ptr<Workspace> mainWorkspace;
    if (backendMode == kBackendModeSDL) {
        _compositor = std::make_shared<SDLCompositor>();
        mainWorkspace = make_shared<SDLWorkspace>(_compositor);
    }
    else if (backendMode == kBackendModeGLFW) {
        _compositor = std::make_shared<GLCompositor>();
        mainWorkspace = make_shared<GlfwWorkspace>(_compositor);
    }
    _workspaces.push_back(mainWorkspace);
    
    return mainWorkspace->run();
}

void Server::addApp(std::shared_ptr<App> app)
{
    _apps.push_back(app);
}

void Server::removeAppById(TAppId id)
{
    for (auto iter = _apps.begin(); iter != _apps.end(); ++iter) {
        std::shared_ptr<App> a = *iter;
        if (a->getId() == id) {
            _compositor->removeWindows(id);
            _apps.erase(iter);
            return;
        }
    }
}

std::weak_ptr<App> Server::findApp(TAppId id) const
{
    for (auto iter = _apps.begin(); iter != _apps.end(); ++iter) {
        std::shared_ptr<App> app = *iter;
        if (app->getId() == id) {
            return app;
        }
    }
    return std::weak_ptr<App>();
}

std::weak_ptr<Compositor> Server::getCompositor() const
{
    return _compositor;
}

std::weak_ptr<WindowManager> Server::getWindowManager() const
{
    return _windowManager;
}

std::weak_ptr<message_queue> Server::getMessageQueue() const
{
    return _msgq;
}

std::weak_ptr<zmq::context_t> Server::getSocketContext() const
{
    return _context;
}

std::weak_ptr<zmq::socket_t> Server::getSocket() const
{
    return _socket;
}

Server* Server::getSingleton()
{
    if (_sharedInst == nullptr) {
        _sharedInst = new Server;
    }
    return _sharedInst;
}

std::shared_ptr<Workspace> Server::getScreenAt(int index) const
{
    return _workspaces[index];
}

std::shared_ptr<Workspace> Server::getActiveScreen() const
{
    return getScreenAt(0);
}

BackendMode Server::getBackendMode() const
{
    return _backendMode;
}

Server::~Server()
{
    asl::Connector::closeSocket(_sock);
    message_queue::remove(asl::Connector::serverMessageQueueName().c_str());
}

