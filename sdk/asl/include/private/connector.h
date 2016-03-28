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
        TWindowId createWindow(void *data, uint64_t dataSize, double x, double y, double width, double height, uint8_t rasterType);
        void updateWindow(TWindowId windowId, void *data, uint64_t dataSize, double x, double y, double width, double height, bool compression);
        void resizeWindow(TWindowId windowId, void *data, uint64_t dataSize, double width, double height);
        void changeWindowVisiblity(TWindowId windowId, bool visible);
        void bringWindowToFront(TWindowId windowId);
        void moveWindow(TWindowId windowId, double x, double y);
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
