//
//  server.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <pthread.h>
#include <signal.h>
#include <Server.h>
#include <SDLCompositor.h>
#include <SDLEventSource.h>

using namespace appserver;

Server* Server::_sharedInst = nullptr;

Server::Server()
{
    _context = std::make_shared<zmq::context_t>(1);
    _socket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REP);
    _socket->bind("tcp://*:9000");
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
                std::shared_ptr<zmq::socket_t> socket = Server::getSingleton()->getSocket().lock();

                if (req.type == AspRequestRegister) {
                    std::shared_ptr<App> app = nullptr;
                    app = std::make_shared<App>(req.field0);
                    app->startRequestListener();

                    // Send the client ID back to the app
                    Asp_Event evt;
                    evt.field0 = app->getId();
                    zmq::message_t response(&evt, sizeof(Asp_Event));
                    socket->send(response);

                    server->addApp(app);
                }
                else if (req.type == AspRequestUnregister) {
                    server->removeAppByPid(req.field0);

                    // ACK
                    int ack = 1;
                    zmq::message_t ackResponse(&ack, sizeof(int));
                    socket->send(ackResponse);

                    std::cout << "Removed app with pid " << req.field0 << std::endl;
                }
            }
        }
        catch (zmq::error_t e) {
            std::cout << __func__ << ": " << e.what() << std::endl;
        }
    }
    return nullptr;
}

void Server::run()
{
    if (pthread_create(&_messageDispatcher, NULL, Server::requestListener, NULL)) {
        throw std::runtime_error(strerror(errno));
    }
    
    _compositor = std::make_shared<SDLCompositor>();
    _inputSource = std::make_shared<SDLEventSource>();
    _windowManager = std::make_shared<WindowManager>();
    
    while (true) {
        _compositor->compose();
        
        std::shared_ptr<Event> evt = _inputSource->pollEvent();
        if (evt != nullptr) {
            uint8_t eventType = evt->getType();
            if(eventType == AspEventTypeQuit) {
                break;
            }
            else {
                _windowManager->sendEvent(evt);
            }
        }
        
        Server::avoidBusyWait(10 * NANO_SECOND_MULTIPLIER);
    }
}

void Server::avoidBusyWait(const long int nsec)
{
    timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = nsec;
    nanosleep(&tim, NULL);
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

void Server::removeAppByPid(TProcId pid)
{
    for (auto iter = _apps.begin(); iter != _apps.end(); ++iter) {
        std::shared_ptr<App> a = *iter;
        if (a->getPid() == pid) {
            _compositor->removeWindows(a->getId());
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

void Server::setAppsHost(std::string host)
{
    _appsHost = host;
}

std::string Server::getAppsHost() const
{
    return _appsHost;
}

Server::~Server()
{
}

