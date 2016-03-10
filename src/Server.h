//
//  server.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <vector>

#include <Workspace.h>
#include <App.h>
#include <Compositor.h>
#include <WindowManager.h>
#include <Asl/Protocol.h>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <zmq.hpp>

namespace appserver
{
    enum BackendMode {
        kBackendModeSDL,
        kBackendModeGLFW
    };
    
    class Server
    {
    public:
        virtual ~Server();
        void run(BackendMode backendMode);
        std::shared_ptr<Workspace> getActiveScreen() const;
        std::shared_ptr<Workspace> getScreenAt(int index) const;
        void addApp(std::shared_ptr<App> app);
        void removeAppById(TAppId id);
        void removeAppByPid(TProcId pid);
        std::weak_ptr<App> findApp(TAppId id) const;
        std::weak_ptr<Compositor> getCompositor() const;
        std::weak_ptr<WindowManager> getWindowManager() const;
        void dispatchMessage(Asp_Request req);
        std::weak_ptr<message_queue> getMessageQueue() const;
        std::weak_ptr<zmq::context_t> getSocketContext() const;
        std::weak_ptr<zmq::socket_t> getSocket() const;
        BackendMode getBackendMode() const;
        // Static methods:
        static Server* getSingleton();
        static void* requestListener(void *ptr);
        static void* processMonitor(void *ptr);
    private:
        Server();
    private:
        std::vector<std::shared_ptr<Workspace>> _workspaces;
        std::vector<std::shared_ptr<App>> _apps;
        std::shared_ptr<Compositor> _compositor;
        std::shared_ptr<WindowManager> _windowManager;
        std::shared_ptr<message_queue> _msgq;
        static Server* _sharedInst;
        pthread_t _messageDispatcher;
        pthread_t _processMonitor;
        int _sock;
        BackendMode _backendMode;
        // zmq
        std::shared_ptr<zmq::context_t> _context;
        std::shared_ptr<zmq::socket_t> _socket;
    };
}

#endif
