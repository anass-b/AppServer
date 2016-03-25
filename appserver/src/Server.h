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
#include <App.h>
#include <Compositor.h>
#include <WindowManager.h>
#include <InputSource.h>
#include <protocol.h>
#include <zmq.hpp>

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

namespace appserver
{
    const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;
    
    class Server
    {
    public:
        static Server* getSingleton();
        virtual ~Server();
        void run();
        void addApp(std::shared_ptr<App> app);
        void removeAppById(TAppId id);
        void removeAppByPid(TProcId pid);
        std::weak_ptr<App> findApp(TAppId id) const;
        std::weak_ptr<Compositor> getCompositor() const;
        std::weak_ptr<WindowManager> getWindowManager() const;
        std::weak_ptr<zmq::context_t> getSocketContext() const;
        std::weak_ptr<zmq::socket_t> getSocket() const;
        void setAppsHost(std::string host);
        std::string getAppsHost() const;
        static void avoidBusyWait(const long nsec = INTERVAL_MS);
    private:
        Server();
        static void* requestListener(void *ptr);
        static void* processMonitor(void *ptr);
    private:
        std::vector<std::shared_ptr<App>> _apps;
        std::shared_ptr<InputSource> _inputSource;
        std::shared_ptr<Compositor> _compositor;
        std::shared_ptr<WindowManager> _windowManager;
        static Server* _sharedInst;
        pthread_t _messageDispatcher;
        int _sock;
        std::string _appsHost;
        std::shared_ptr<zmq::context_t> _context;
        std::shared_ptr<zmq::socket_t> _socket;
    };
}

#endif
