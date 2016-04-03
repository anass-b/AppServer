//
//  App.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef APP_H
#define APP_H

#include <iostream>
#include <memory>
#include <vector>
#include <string.h>
#include <zmq.hpp>
#include <window.h>
#include <events.h>
#include <thread>

namespace appserver
{
    class App : public std::enable_shared_from_this<App>
    {
    public:
        friend class Compositor;
        friend class Window;
        App(TProcId pid);
        virtual ~App();
        void sendMouseMoveEvent(std::shared_ptr<MouseMoveEvent> evt, std::shared_ptr<Window> window);
        void sendMouseButtonEvent(std::shared_ptr<MouseButtonEvent> evt, std::shared_ptr<Window> window);
        void sendMouseScrollEvent(std::shared_ptr<MouseScrollEvent> evt, std::shared_ptr<Window> window);
        void sendTextEvent(std::shared_ptr<TextEvent> evt, std::shared_ptr<Window> window);
        void sendKeyEvent(std::shared_ptr<KeyEvent> evt, std::shared_ptr<Window> window);
        TProcId getPid() const;
        TAppId getId() const;        
        std::weak_ptr<zmq::socket_t> getSocket() const;
        std::weak_ptr<zmq::socket_t> getEventSocket() const;
        void startRequestListener();
    protected:
        void newWindow(Asp_Request msg);
        void updateWindow(Asp_Request msg);
        void resizeWindow(Asp_Request msg);
        void setWindowVisibility(Asp_Request msg);
        void moveWindow(Asp_Request msg);
        void bringWindowToFront(Asp_Request msg);
        void destroyWindow(Asp_Request msg);
    private:
        static void* requestListener(void* arg);
        void processMessage(Asp_Request msg);
        void printException(const std::exception &e) const;
        bool sendAck(std::shared_ptr<zmq::socket_t> socket);
        bool recvAck(std::shared_ptr<zmq::socket_t> socket);
    private:
        TAppId _id;
        TProcId _pid;
        static unsigned long _counter;
        bool _busy;
        std::shared_ptr<std::thread> _requestListener;
        std::shared_ptr<zmq::context_t> _context;
        std::shared_ptr<zmq::socket_t> _eventSocket;
        std::shared_ptr<zmq::socket_t> _socket;
        bool _runRequestListener = true;
    };
}

#endif
