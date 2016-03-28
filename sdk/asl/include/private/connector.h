#ifndef ASL_CONNECTOR_H
#define ASL_CONNECTOR_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <zmq.hpp>

#include <protocol.h>
#include <events.h>

using namespace appserver;

namespace asl
{
    class Connector
    {
    public:
        Connector();
        virtual ~Connector();
        void subscribe();
        void unsubscribe();
        TWindowId newWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, int rasterType);
        void updateWindowSurface(TWindowId id, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height);
        void resizeWindow(TWindowId id, unsigned char *data, unsigned long dataSize, double width, double height);
        void changeWindowVisiblity(TWindowId id, bool visible);
        void bringWindowToFront(TWindowId id);
        void moveWindow(TWindowId id, double x, double y);
        void destroyWindow(TWindowId id);
        std::shared_ptr<Event> waitEvent();
    private:
        bool sendAck(std::shared_ptr<zmq::socket_t> socket);
        bool recvAck(std::shared_ptr<zmq::socket_t> socket);
        void printException(const std::exception &e) const;
    private:
        TAppId _clientId;
        std::vector<TWindowId> _windowIds;
        std::shared_ptr<zmq::context_t> _context;
        std::shared_ptr<zmq::socket_t> _regSocket;
        std::shared_ptr<zmq::socket_t> _socket;
        std::shared_ptr<zmq::socket_t> _processMonitorSocket;
        std::shared_ptr<zmq::socket_t> _eventsSocket;
    };
}

#endif
