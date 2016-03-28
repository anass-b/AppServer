/*
 * src/asc.c
 *
 * AppServer Library
 *
 * Copyright (C) 2014  Anass Bouassaba
 *
 */

#include <private/connector.h>
#include <sstream>
#include <unistd.h>
#include <zlib.h>
#include <minilzo/minilzo.h>

#define OS_X 1
#define EXPORT __attribute__((visibility("default")))

int errno;

typedef struct Asp_Event Asp_Event;

using namespace asl;

/*
 * Initiates a socket connection with the server.
 *
 * cl: client
 * return: Negative number if it fails
 */
Connector::Connector()
{  
    _context = std::make_shared<zmq::context_t>();

    if (lzo_init() != LZO_E_OK) {
        exit(1);
    }
}

/*
 * Register the client with the server.
 * Before calling this function, the client won't be able to do any operation on the server.
 */
void Connector::subscribe()
{
    TProcId pid = getpid();

    // Connect to process monitor's socket
    _processMonitorSocket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REQ);
    _processMonitorSocket->connect("tcp://localhost:9001");

    // Send pid to process monitor
    Asp_SubscribeRequest req;
    req.pid = pid;
    zmq::message_t processMonitorRequest(&req, sizeof(Asp_SubscribeRequest));
    _processMonitorSocket->send(processMonitorRequest);

    // Receive appserver address size from process monitor
    size_t addrSize;
    _processMonitorSocket->recv(&addrSize, sizeof(size_t));
    if (addrSize <= 0 && addrSize > 15) exit(1);

    sendAck(_processMonitorSocket);

    // Receive the actual appserver addrss from process monitor (IPv4 or localhost)
    char* addr = new char[addrSize + 1];
    _processMonitorSocket->recv(addr, addrSize);
    addr[addrSize] = 0;

    _appServerAddr = addr;

    // Connect to appserver's registration socket
    _regSocket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REQ);
    std::stringstream regSocketAddr;
    regSocketAddr << "tcp://" << _appServerAddr << ":9000";
    _regSocket->connect(regSocketAddr.str());

    // Subscribe to appserver
    Asp_Request subscribeRequest;
    subscribeRequest.type = AspRequestRegister;
    subscribeRequest.field0 = pid;
    zmq::message_t request(&subscribeRequest, sizeof(subscribeRequest));
    _regSocket->send(request);
    Asp_Event evt;
    size_t receivedSize = _regSocket->recv(&evt, sizeof(evt));
    if (receivedSize <= 0) exit(1);
    _clientId = evt.field0;

    // Connect to appserver's request socket
    _socket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REQ);
    std::stringstream reqSocketAddress;
    reqSocketAddress << "tcp://" << _appServerAddr << ":";
    int reqSocketPort = AspReqListenerThreadPortValue + _clientId;
    reqSocketAddress << reqSocketPort;
    _socket->connect(reqSocketAddress.str());
    std::cout << "Connect to appserver's request socket' on " << reqSocketAddress.str() << std::endl;

    // Events socket
    int port = AspEventSocketPortValue + _clientId;
    _eventsSocket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REP);
    std::stringstream address;

    address << "tcp://*:" << port;
    _eventsSocket->bind(address.str());
}

/*
 * Create a window on the server with a initial raster data
 *
 * field0   : X
 * field1   : Y
 * field2   : Width
 * field3   : Height
 * field4   : Visibility
 *
 * return: ID of the created window
 */

TWindowId Connector::createWindow(void *data, uint64_t dataSize, double x, double y, double width, double height, uint8_t rasterType)
{
    try {
        Asp_Request createWindowReq;
        createWindowReq.type = AspRequestCreateWindow;
        createWindowReq.clientId = _clientId;
        createWindowReq.field0 = x;
        createWindowReq.field1 = y;
        createWindowReq.field2 = width;
        createWindowReq.field3 = height;
        createWindowReq.field4 = (int)rasterType;
        createWindowReq.dataSize = dataSize;
        
        // Send request
        zmq::message_t request(&createWindowReq, sizeof(Asp_Request));
        _socket->send(request);
        
        recvAck(_socket);
        
        // Send raster data
        const void* castedData = (const void*)data;
        zmq::message_t dataRequest(castedData, (size_t)dataSize);
        _socket->send(dataRequest);
        
        Asp_Event evt;
        evt.winId = 0;
        unsigned int priority;
        bool foundExistingId = false;
        
        do  {
            size_t receivedSize = _socket->recv(&evt, sizeof(evt));
            if (receivedSize <= 0) return -1;
            
            foundExistingId = false;
            for (int i = 0; i < _windowIds.size(); i++) {
                TWindowId id = _windowIds.at(i);
                if (evt.winId == id) {
                    foundExistingId = true;
                    break;
                }
            }
        } while (foundExistingId || evt.winId == 0);
        
        _windowIds.push_back(evt.winId);
        
        return evt.winId;
    }
    catch (std::exception e) {
        this->printException(e);
    }
    return -1;
}

/*
 * Update window's raster in a specific region.
 * The width and height region should match the arguments width and height given to the function
 *
 * field1   : X
 * field2   : Y
 * field3   : Width
 * field4   : Height
 */
void Connector::updateWindow(TWindowId windowId, void *data, uint64_t dataSize, double x, double y, double width, double height, bool compression)
{
    try {
        unsigned char* compressedData = nullptr;
        void* wrkmem = nullptr;
        lzo_uint compressedSize = 0;
        if (compression) {
            // Compress data
            compressedData = new unsigned char[dataSize];
            wrkmem = malloc(LZO1X_1_MEM_COMPRESS);
            int r = lzo1x_1_compress((unsigned char*)data, dataSize, compressedData, &compressedSize, wrkmem);
            if (r != LZO_E_OK || compressedSize >= dataSize) {
                return;
            }
        }

        Asp_Request req;
        req.type = AspRequestUpdateWindowSurface;
        req.clientId = _clientId;
        req.winId = windowId;
        req.field0 = x;
        req.field1 = y;
        req.field2 = width;
        req.field3 = height;
        req.field4 = compression;
        req.dataSize = dataSize;
        req.compressedSize = compression ? compressedSize : 0;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
        
        recvAck(_socket);
        
        // Send raster data
        if (compression) {
            zmq::message_t dataRequest(compressedData, (size_t)compressedSize);
            _socket->send(dataRequest);
        }
        else {
            zmq::message_t dataRequest(data, (size_t)dataSize);
            _socket->send(dataRequest);
        }

        recvAck(_socket);

        if (compression) {
            delete[] compressedData;
            free(wrkmem);
        }
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

/*
 * Resize window
 *
 * field1   : Width
 * field2   : Height
 */
void Connector::resizeWindow(TWindowId windowId, void *data, uint64_t dataSize, double width, double height)
{
    try {
        Asp_Request req;
        req.type = AspRequestResizeWindow;
        req.clientId = _clientId;
        req.winId = windowId;
        req.field0 = width;
        req.field1 = height;
        req.dataSize = dataSize;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
        
        recvAck(_socket);
        
        // Send raster data
        const void* castedData = (const void*)data;
        zmq::message_t dataRequest(castedData, (size_t)dataSize);
        _socket->send(dataRequest);
        
        recvAck(_socket);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

/*
 * Show or hide a window
 *
 * field1 : Visiblility
 */
void Connector::changeWindowVisiblity(TWindowId id, bool visible)
{
    try {
        Asp_Request req;
        req.type = AspRequestSetWindowVisibility;
        req.clientId = _clientId;
        req.winId = id;
        req.field0 = visible;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

/*
 * Make a window top level
 */
void Connector::bringWindowToFront(TWindowId id)
{
    try {
        Asp_Request req;
        req.type = AspRequestBringWindowToFront;
        req.clientId = _clientId;
        req.winId = id;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

/*
 * Change window's location
 *
 * field1 : X
 * field2 : Y
 */
void Connector::moveWindow(TWindowId id, double x, double y)
{
    try {
        Asp_Request req;
        req.type = AspRequestMoveWindow;
        req.clientId = _clientId;
        req.winId = id;
        req.field0 = x;
        req.field1 = y;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

/*
 * Destroy window
 */
void Connector::destroyWindow(TWindowId id)
{
    try {
        Asp_Request req;
        req.type = AspRequestDestroyWindow;
        req.clientId = _clientId;
        req.winId = id;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
    }
    catch (std::exception e) {
        this->printException(e);
    }
}

/*
 * A blocker function that returns an input event from the server when available
 *
 * return: An event from the server
 */
std::shared_ptr<Event> Connector::waitEvent()
{
    try {
        Asp_Event req;
        size_t receivedSize = _eventsSocket->recv(&req, sizeof(Asp_Event));
        if (receivedSize == 0) {
            exit(1);
        }
        
        sendAck(_eventsSocket);
        
        if (req.type == AspEventTypeMouseMove) {
            return std::make_shared<MouseMoveEvent>(req);
        }
        else if (req.type == AspEventTypeMouseButton) {
            return std::make_shared<MouseButtonEvent>(req);
        }
        else if (req.type == AspEventTypeMouseScroll) {
            return std::make_shared<MouseScrollEvent>(req);
        }
        else if (req.type == AspEventTypeKey) {
            return std::make_shared<KeyEvent>(req);
        }
        else if (req.type == AspEventTypeText) {
            char *text = (char*)malloc(req.field0);
            receivedSize = _eventsSocket->recv(text, req.field0 - 1);
            text[(int)(req.field0 - 1)] = 0;
            if (receivedSize == 0) {
                exit(1);
            }
            
            sendAck(_eventsSocket);
            
            std::shared_ptr<TextEvent> textEvent = std::make_shared<TextEvent>(req);
            textEvent->setText(text);

            return textEvent;
        }
    }
    catch (zmq::error_t e) {
        this->printException(e);
    }
    
    return nullptr;
}

void Connector::unsubscribe()
{    
    _socket->close();
}

bool Connector::sendAck(std::shared_ptr<zmq::socket_t> socket)
{
    uint8_t ack = 1;
    zmq::message_t msg(&ack, sizeof(uint8_t));
    return socket->send(msg);
}

bool Connector::recvAck(std::shared_ptr<zmq::socket_t> socket)
{
    uint8_t ack = 0;
    size_t receivedSize = socket->recv(&ack, sizeof(uint8_t));
    if (receivedSize <= 0 || ack != 1) {
        return false;
    }
    return true;
}

void Connector::printException(const std::exception &e) const
{
    std::cout << e.what() << std::endl;
}

Connector::~Connector()
{
}

