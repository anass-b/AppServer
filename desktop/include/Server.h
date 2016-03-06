//
//  server.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_server_h
#define appserver_server_h

#include <memory>
#include <vector>

#include <Workspace.h>
#include <App.h>
#include <Compositor.h>
#include <WindowManager.h>
#include <Asl/Protocol.h>
#include <boost/interprocess/ipc/message_queue.hpp>

namespace appserver
{
    class Server
    {
    public:
        virtual ~Server();
        void run();
        std::shared_ptr<Workspace> getActiveScreen() const;
        std::shared_ptr<Workspace> getScreenAt(int index) const;
        void addApp(std::shared_ptr<App> app);
        void removeAppById(TAppId id);
        std::weak_ptr<App> findApp(TAppId id) const;
        std::weak_ptr<Compositor> getCompositor() const;
        std::weak_ptr<WindowManager> getWindowManager() const;
        void dispatchMessage(Asp_Request req);
        std::weak_ptr<message_queue> getMessageQueue() const;
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
    };
}

#endif
