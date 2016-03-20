//
//  App.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_App_h
#define appserver_App_h

#include <iostream>
#include <memory>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <zmq.hpp>
#include <Window.h>

using namespace boost::interprocess;

namespace appserver
{
    class App : public std::enable_shared_from_this<App>
    {
    public:
        friend class Compositor;
        friend class Window;
        App(TProcId pid);
        virtual ~App();
        void createAndConnectSocket();
        void sendMouseMoveEvent(TWindowId windowId, int type, double x, double y, double absX, double absY);
        void sendMouseButtonEvent(TWindowId windowId, int type, int button, double x, double y, double absX, double absY);
        void sendKeyEvent(TWindowId windowId, int charCode);
        TProcId getPid() const;
        TAppId getId() const;
        void processMessage(Asp_Request msg);
        std::weak_ptr<message_queue> getMessageQueue() const;
        std::weak_ptr<zmq::socket_t> getSocket() const;
    protected:
        void newWindow(Asp_Request msg);
        void updateWindow(Asp_Request msg);
        void resizeWindow(Asp_Request msg);
        void setWindowVisibility(Asp_Request msg);
        void moveWindow(Asp_Request msg);
        void bringWindowToFront(Asp_Request msg);
        void destroyWindow(Asp_Request msg);
    private:
        TAppId _id;
        TProcId _pid;
        static unsigned long _counter;
        bool _busy;
        std::shared_ptr<message_queue> _msgq;
        // zmq
        std::shared_ptr<zmq::socket_t> _socket; 
    };
}

#endif
