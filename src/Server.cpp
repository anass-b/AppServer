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
#include <Asl/Logger.h>
#include <Asl/Connector.h>
#include <Asl/Utils.h>

#include <pthread.h>
#include <signal.h>

using namespace appserver;

Server* Server::_sharedInst = nullptr;

Server::Server()
{
    message_queue::remove(asl::Connector::serverMessageQueueName().c_str());
    _msgq = make_shared<message_queue>(create_only, asl::Connector::serverMessageQueueName().c_str(), 1000000, sizeof(Asp_Request));
}

void Server::dispatchMessage(Asp_Request req)
{
    if (req.type == AspRequestRegister) {
        std::shared_ptr<App> app = nullptr;
        try {
            app = make_shared<App>(req.field0);
            Asp_Event evt;
            evt.field0 = app->getId();
            app->getMessageQueue().lock()->send(&evt, sizeof(evt), 0);
            addApp(app);
        } catch (std::runtime_error e) {
            std::string what(e.what());
            asl::Logger::log("Cannot register client app: " + what);
            removeAppById(app->getId());
        }
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
            asl::Logger::log("Reuqested app not found");
        }
    }
}

void* Server::requestListener(void *ptr)
{
    Server* server = Server::getSingleton();
    std::shared_ptr<message_queue> msgq = server->getMessageQueue().lock();
    for (;;) {
        try {
            Asp_Request req;
            unsigned int priority;
            message_queue::size_type receivedSize;
            msgq->receive(&req, sizeof(Asp_Request), receivedSize, priority);
            if (receivedSize > 0) {
                server->dispatchMessage(req);
            }
        } catch (exception e) {
            cout << e.what() << endl;
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

void Server::run()
{
    if (pthread_create(&_messageDispatcher, NULL, Server::requestListener, NULL)) {
        throw runtime_error(strerror(errno));
    }
    
    if (pthread_create(&_processMonitor, NULL, Server::processMonitor, NULL)) {
        throw runtime_error(strerror(errno));
    }
    
    _compositor = std::make_shared<SDLCompositor>();
    _windowManager = std::make_shared<WindowManager>();
    
    std::shared_ptr<Workspace> mainWorkspace = make_shared<SDLWorkspace>(_compositor);
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

Server::~Server()
{
    asl::Connector::closeSocket(_sock);
    message_queue::remove(asl::Connector::serverMessageQueueName().c_str());
}

