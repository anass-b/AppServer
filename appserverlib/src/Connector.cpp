/*
 * src/asc.c
 *
 * AppServer Library
 *
 * Copyright (C) 2014  Anass Bouassaba
 *
 */

#include <Asl/Connector.h>
#include <Asl/Logger.h>
#include <sstream>
#include <unistd.h>

#define OS_X 1
#define EXPORT __attribute__((visibility("default")))

int errno;

typedef struct Asp_Event Asp_Event;

using namespace asl;

// ------------------------------------ Event

Event::Event()
    : _windowId(AspUndefinedWindowId), _eventType(kEventTypeUndefined)
{
}

Event::Event(TWindowId id, EventType type)
    : _windowId(id), _eventType(type)
{
}

void Event::setWindowId(TWindowId id)
{
    _windowId = id;
}

TWindowId Event::getWindowId() const
{
    return _windowId;
}

void Event::setEventType(EventType type)
{
    _eventType = type;
}

EventType Event::getEventType() const
{
    return _eventType;
}

Event::~Event()
{
}

// ------------------------------------ InputEvent

InputEvent::InputEvent()
    : Event(), _inputEventType(kInputEventTypeUndefined)
{
}

InputEvent::InputEvent(TWindowId id, InputEventType type)
    : Event(id, kEventTypeInput), _inputEventType(type)
{
}

void InputEvent::setInputEventType(InputEventType type)
{
    _inputEventType = type;
}

InputEventType InputEvent::getInputEventType() const
{
    return _inputEventType;
}

InputEvent::~InputEvent()
{
}

// ------------------------------------ WindowLocationChangedEvent

WindowLocationChangedEvent::WindowLocationChangedEvent(const Asp_Event& aspEvent)
: Event(aspEvent.winId, kEventTypeWindowLocationChanged), _x(aspEvent.field0), _y(aspEvent.field1)
{
}

WindowLocationChangedEvent::WindowLocationChangedEvent()
: Event(), _x(0.0), _y(0.0)
{
}

WindowLocationChangedEvent::WindowLocationChangedEvent(TWindowId id, double x, double y)
: Event(id, kEventTypeWindowLocationChanged), _x(x), _y(y)
{
}

void WindowLocationChangedEvent::setNewWindowX(double x)
{
    _x = x;
}

void WindowLocationChangedEvent::setNewWindowY(double y)
{
    _y = y;
}

double WindowLocationChangedEvent::getNewWindowX() const
{
    return _x;
}

double WindowLocationChangedEvent::getNewWindowY() const
{
    return _y;
}

WindowLocationChangedEvent::~WindowLocationChangedEvent()
{
}

// ------------------------------------ MouseEvent

MouseEvent::MouseEvent()
    : InputEvent(AspUndefinedWindowId, kInputEventTypeMouse),
    _mouseEventType(kMouseEventTypeUndefined), _button(kMouseButtonUndefined), _x(0.0f), _y(0.0f), _absX(0.0f), _absY(0.0f)
{
}

MouseEvent::MouseEvent(TWindowId id, MouseEventType type, MouseButton button, double x, double y, double absX, double absY)
    : InputEvent(id, kInputEventTypeMouse), _mouseEventType(type), _button(button), _x(x), _y(y), _absX(absX), _absY(absY)
{
}

MouseEvent::MouseEvent(const Asp_Event& aspEvent)
    : InputEvent(AspUndefinedWindowId, kInputEventTypeMouse)
{
    setWindowId(aspEvent.winId);
    
    if (aspEvent.field4 == AspMouseEventMove) {
        setMouseEventType(kMouseEventTypeMove);
        setMouseButton(kMouseButtonUndefined);
    }
    else if (aspEvent.field4 == AspMouseEventPress) {
        setMouseEventType(kMouseEventTypePress);
        setMouseButton(parseMouseButton(aspEvent.field5));
    }
    else if (aspEvent.field4 == AspMouseEventRelease) {
        setMouseEventType(kMouseEventTypeRelease);
        setMouseButton(parseMouseButton(aspEvent.field5));
    }
    else if (aspEvent.field4 == AspMouseEventDrag) {
        setMouseEventType(kMouseEventTypeDrag);
        setMouseButton(parseMouseButton(aspEvent.field5));
    }
    
    setX(aspEvent.field0);
    setY(aspEvent.field1);
    setAbsX(aspEvent.field2);
    setAbsY(aspEvent.field3);
}

MouseButton MouseEvent::parseMouseButton(unsigned int aspMouseButton)
{
    if (aspMouseButton == AspMouseButtonRight) {
        return kMouseButtonRight;
    }
    else if (aspMouseButton == AspMouseButtonMiddle) {
        return kMouseButtonMiddle;
    }
    else if (aspMouseButton == AspMouseButtonLeft) {
        return kMouseButtonLeft;
    }
    
    return kMouseButtonUndefined;
}

void MouseEvent::setMouseEventType(MouseEventType type)
{
    _mouseEventType = type;
}

MouseEventType MouseEvent::getMouseEventType() const
{
    return _mouseEventType;
}

void MouseEvent::setMouseButton(MouseButton button)
{
    _button = button;
}

MouseButton MouseEvent::getMouseButton() const
{
    return _button;
}

void MouseEvent::setX(double x)
{
    _x = x;
}

double MouseEvent::getX() const
{
    return _x;
}

void MouseEvent::setY(double y)
{
    _y = y;
}

double MouseEvent::getY() const
{
    return _y;
}

void MouseEvent::setAbsX(double absX)
{
    _absX = absX;
}

double MouseEvent::getAbsX() const
{
    return _absX;
}

void MouseEvent::setAbsY(double absY)
{
    _absY = absY;
}

double MouseEvent::getAbsY() const
{
    return _absY;
}

MouseEvent::~MouseEvent()
{
}

// ------------------------------------ KeyEvent

KeyEvent::KeyEvent()
{
}

KeyEvent::KeyEvent(TWindowId id, KeyEventType type, uint32_t charCode)
    : InputEvent(id, kInputEventTypeKey), _keyEventType(type), _charCode(charCode)
{
}

KeyEvent::KeyEvent(const Asp_Event& aspEvent)
    : InputEvent(AspUndefinedWindowId, kInputEventTypeKey)
{
    setWindowId(aspEvent.winId);
    setCharCode(aspEvent.field0);
}

void KeyEvent::setKeyEventType(KeyEventType type)
{
    _keyEventType = type;
}

KeyEventType KeyEvent::getKeyEventType() const
{
    return _keyEventType;
}

void KeyEvent::setCharCode(uint32_t code)
{
    _charCode = code;
}

uint32_t KeyEvent::getCharCode() const
{
    return _charCode;
}

KeyEvent::~KeyEvent()
{    
}

// ------------------------------------ Connector

const int Connector::kMsgType = 1;

/*
 * Initiates a socket connection with the server.
 *
 * cl: client
 * return: Negative number if it fails
 */
Connector::Connector()
{
    int pid = getpid();
  
    _context = std::make_shared<zmq::context_t>(1);
    _socket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REQ);
    _socket->connect ("tcp://192.168.1.3:5555");
    
    // Events socket
    //_eventsContext = std::make_shared<zmq::context_t>(1);
    _eventsSocket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REP);
    _eventsSocket->bind("tcp://*:6666");
}

#define OS_MacOSX 1

int Connector::listen(const char *sockPath)
{
    int sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        throw runtime_error(strerror(errno));
    }
    
    struct sockaddr_un localAddr;
    localAddr.sun_family = AF_UNIX;
    ::strcpy(localAddr.sun_path, sockPath);
    ::unlink(localAddr.sun_path);
    
#ifdef OS_MacOSX
    if (::bind(sock, (struct sockaddr*)&localAddr, (socklen_t)SUN_LEN(&localAddr)) == -1)
#else
        socklen_t sockLen = (socklen_t)strlen(localAddr.sun_path) + (socklen_t)sizeof(localAddr.sun_family);
    if (::bind(_sock, (struct sockaddr*)&localAddr, sockLen) == -1)
#endif
    {
        ::close(sock);
        throw runtime_error(strerror(errno));
    }
    
    if (::listen(sock, 256) == -1)
    {
        ::close(sock);
        throw runtime_error(strerror(errno));
    }
    
    return sock;
}

int Connector::accept(int sock)
{
    int remoteSock;
    socklen_t len = sizeof(remoteSock);
    if ((remoteSock = ::accept(sock, (struct sockaddr*)&remoteSock, &len)) == -1) {
        throw runtime_error(strerror(errno));
    }
    return remoteSock;
}

int Connector::connect(const char *sockPath)
{
    int sock;
    if ((sock = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket1");
    }
    
    struct sockaddr_un remoteAddr;
    remoteAddr.sun_family = AF_UNIX;
    strcpy(remoteAddr.sun_path, sockPath);
#ifdef OS_X
    if (::connect(sock, (struct sockaddr *)&remoteAddr, (socklen_t)SUN_LEN(&remoteAddr)) == -1)
#else
    socklen_t len = (socklen_t)strlen(remoteAddr.sun_path) + (socklen_t)sizeof(cl->remoteAddr.sun_family);
    if (::connect(sock, (struct sockaddr *)&remote_addr, len) == -1)
#endif
    {
        perror("connect1");
        exit(1);
    }
    
    return sock;
}

size_t Connector::sendAll(int socket, unsigned char *buffer, size_t length, int flags)
{
    size_t readSum = 0;
    ssize_t n = -1;
    unsigned char *p = buffer;
    while (length > 0) {
        n = send(socket, p, length, flags);
        if (n == -1) {
            printf("sendAll: %s\n", strerror(errno));
            return readSum;
        }
        p += n;
        length -= n;
        readSum += n;
    }
    return readSum;
}

size_t Connector::receiveAll(int sockfd, void *buf, size_t len, int flags)
{
    size_t toread = len;
    unsigned char  *bufptr = (unsigned char*) buf;
    size_t read_sum = 0;
    
    while (toread > 0)
    {
        ssize_t rsz = recv(sockfd, bufptr, toread, flags);
        if (rsz == -1) {
            perror("recv");
            return read_sum;
        }
        else if (rsz == 0) {
            return read_sum;
        }
        
        // Read less next time
        toread -= rsz;
        // Next buffer position to read into
        bufptr += rsz;
        read_sum += rsz;
    }
    
    return read_sum;
}

void Connector::closeSocket(int sock)
{
    ::close(sock);
}

void Connector::shutdownSocket(int sock)
{
    ::shutdown(sock, SHUT_RDWR);
}

std::string Connector::serverSocketPath()
{
    return "/tmp/appsrv_sock";
}

std::string Connector::serverMessageQueueName()
{
    return "appsrvmq";
}

std::string Connector::messageQueueNameFromPid(TProcId pid)
{
    ostringstream name;
    name << "appmq_" << pid;
    
    return name.str();
}

std::string Connector::sockPathFromPid(TProcId pid)
{
    ostringstream name;
    name << "/tmp/appsock_" << pid;
    
    return name.str();
}

/*
 * Register the client with the server.
 * Before calling this function, the client won't be able to do any operation on the server.
 */
void Connector::subscribe()
{
    Asp_Request subscribeRequest;
    subscribeRequest.type = AspRequestRegister;
    subscribeRequest.field0 = getpid();
    
    zmq::message_t request(&subscribeRequest, sizeof(subscribeRequest));
    _socket->send(request);
    
    Asp_Event evt;
    size_t receivedSize = _socket->recv(&evt, sizeof(evt));
    if (receivedSize > 0) {  
        _clientId = evt.field0;
        std::cout << "Received client ID: " << _clientId << std::endl;
    }
    else {
        exit(1);
    }
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

TWindowId Connector::newWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, int rasterType, bool visible)
{
    try {
        Asp_Request createWindowReq;
        createWindowReq.type = AspRequestCreateWindow;
        createWindowReq.clientId = _clientId;
        createWindowReq.field0 = x;
        createWindowReq.field1 = y;
        createWindowReq.field2 = width;
        createWindowReq.field3 = height;
        createWindowReq.field4 = (bool)visible;
        createWindowReq.field5 = (int)rasterType;
        createWindowReq.dataSize = dataSize;
        
        // Send request
        zmq::message_t request(&createWindowReq, sizeof(Asp_Request));
        _socket->send(request);
        
        // ACK
        int ack = 0;
        size_t receivedSize = _socket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return -1;
        }
        std::cout << "newWindow: got ACK for newWindow" << std::endl;
        
        // Send raster data
        const void* castedData = (const void*)data;
        zmq::message_t dataRequest(castedData, (size_t)dataSize);
        _socket->send(dataRequest);
        
        std::cout << "newWindow: data sent" << std::endl;
        
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
            
            if (foundExistingId) {
                Logger::log(__func__, "Got the same window_id like previous time. Iterating again...");
            }
            else if (evt.winId == 0) {
                Logger::log(__func__, "Window_id equals 0. Iterating again...");
            }
        } while (foundExistingId || evt.winId == 0);
        
        std::cout << "got window ID: " << evt.winId << std::endl;
        _windowIds.push_back(evt.winId);
        
        return evt.winId;
    }
    catch (std::exception e) {
        Logger::log(__func__, e.what());
        return -1;
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
void Connector::updateWindowSurface(TWindowId id, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height)
{
    try {
        Asp_Request req;
        req.type = AspRequestUpdateWindowSurface;
        req.clientId = _clientId;
        req.winId = id;
        req.field1 = x;
        req.field2 = y;
        req.field3 = width;
        req.field4 = height;
        req.dataSize = dataSize;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
        
        // ACK
        int ack = 0;
        size_t receivedSize = _socket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
        
        // Send raster data
        const void* castedData = (const void*)data;
        zmq::message_t dataRequest(castedData, (size_t)dataSize);
        _socket->send(dataRequest);
        
        // ACK
        ack = 0;
        receivedSize = _socket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
    }
    catch (exception e) {
        Logger::log(__func__, e.what());
    }
}

/*
 * Resize window
 *
 * field1   : Width
 * field2   : Height
 */
void Connector::resizeWindow(TWindowId id, unsigned char *data, unsigned long dataSize, double width, double height)
{
    try {
        Asp_Request req;
        req.type = AspRequestResizeWindow;
        req.clientId = _clientId;
        req.winId = id;
        req.field1 = width;
        req.field2 = height;
        req.dataSize = dataSize;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
        
        // ACK
        int ack = 0;
        size_t receivedSize = _socket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
        
        // Send raster data
        const void* castedData = (const void*)data;
        zmq::message_t dataRequest(castedData, (size_t)dataSize);
        _socket->send(dataRequest);
        
        // ACK
        ack = 0;
        receivedSize = _socket->recv(&ack, sizeof(int));
        if (receivedSize <= 0 || ack != 1) {
            return;
        }
    }
    catch (std::exception e) {
        Logger::log(__func__, e.what());
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
        req.field1 = visible;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
    }
    catch (std::exception e) {
        Logger::log(__func__, e.what());
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
        Logger::log(__func__, e.what());
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
        req.field1 = x;
        req.field2 = y;
        
        // Send request
        zmq::message_t request(&req, sizeof(Asp_Request));
        _socket->send(request);
    }
    catch (std::exception e) {
        Logger::log(__func__, e.what());
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
        Logger::log(__func__, e.what());
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
        
        int ack = 1;
        zmq::message_t ackResponse(&ack, sizeof(int));
        _eventsSocket->send(ackResponse);
        
        if (req.type == AspEventMouseInput) {
            return make_shared<MouseEvent>(req);
        }
        else if (req.type == AspEventKeyInput) {
            return make_shared<KeyEvent>(req);
        }
        else if (req.type == AspEventWindowLocationChanged) {
            return make_shared<WindowLocationChangedEvent>(req);
        }
    }
    catch (zmq::error_t e) {
        Logger::log(__func__, e.what());
    }
    
    return nullptr;
}

void Connector::unsubscribe()
{
    /*Asp_Request unsubscribeReq;
    unsubscribeReq.type = AspRequestUnregister;
    unsubscribeReq.clientId = _clientId;
    zmq::message_t request(&unsubscribeReq, sizeof(unsubscribeReq));
    _socket->send(request);*/
    
    _socket->close();
}

Connector::~Connector()
{
    //_socket->close();
    //std::cout << "~Connector()" << std::endl;
    
    /*try {
        unsubscribe();
        _msgQueue->remove(_msgQueueName.c_str());
    }
    catch (interprocess_exception e) {
        Logger::log(__func__, e.what());
    }*/
}

