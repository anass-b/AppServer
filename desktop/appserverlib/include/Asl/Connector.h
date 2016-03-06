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
#include <Asl/Protocol.h>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

namespace asl
{
    typedef enum {
        kMouseEventTypeUndefined,
        kMouseEventTypePress,
        kMouseEventTypeRelease,
        kMouseEventTypeMove,
        kMouseEventTypeDrag
    } MouseEventType;
    
    typedef enum {
        kMouseButtonUndefined,
        kMouseButtonRight,
        kMouseButtonLeft,
        kMouseButtonMiddle
    } MouseButton;
    
    typedef enum {
        kKeyEventTypeUndefined,
        kKeyEventTypePress,
        kKeyEventTypeRelease
    } KeyEventType;

    typedef enum {
        kInputEventTypeUndefined,
        kInputEventTypeMouse,
        kInputEventTypeKey
    } InputEventType;
    
    typedef enum {
        kEventTypeUndefined,
        kEventTypeInput,
        kEventTypeWindowLocationChanged
    } EventType;
    
    class Event
    {
    public:
        Event();
        Event(TWindowId id, EventType type);
        virtual ~Event();
        void setWindowId(TWindowId id);
        TWindowId getWindowId() const;
        void setEventType(EventType type);
        EventType getEventType() const;
    protected:
        TWindowId _windowId;
        EventType _eventType;
    };
    
    class InputEvent : public Event
    {
    public:
        InputEvent();
        InputEvent(TWindowId id, InputEventType type);
        virtual ~InputEvent();
        void setInputEventType(InputEventType type);
        InputEventType getInputEventType() const;
    protected:
        InputEventType _inputEventType;
    };
    
    class WindowLocationChangedEvent : public Event
    {
    public:
        WindowLocationChangedEvent(const Asp_Event& aspEvent);
        WindowLocationChangedEvent();
        WindowLocationChangedEvent(TWindowId id, double x, double y);
        virtual ~WindowLocationChangedEvent();
        void setNewWindowX(double x);
        void setNewWindowY(double y);
        double getNewWindowX() const;
        double getNewWindowY() const;
    protected:
        double _x;
        double _y;
    };
    
    class MouseEvent : public InputEvent
    {
    public:
        MouseEvent();
        MouseEvent(const Asp_Event& aspEvent);
        MouseEvent(TWindowId id, MouseEventType type, MouseButton button, double x, double y, double absX, double absY);
        virtual ~MouseEvent();
        void setMouseEventType(MouseEventType type);
        MouseEventType getMouseEventType() const;
        void setMouseButton(MouseButton button);
        MouseButton getMouseButton() const;
        void setX(double x);
        double getX() const;
        void setY(double y);
        double getY() const;
        void setAbsX(double absX);
        double getAbsX() const;
        void setAbsY(double absY);
        double getAbsY() const;
    protected:
        MouseButton parseMouseButton(unsigned int aspMouseButton);
    protected:
        MouseEventType _mouseEventType;
        MouseButton _button;
        double _x;
        double _y;
        double _absX;
        double _absY;
    };
    
    class KeyEvent : public InputEvent
    {
    public:
        KeyEvent();
        KeyEvent(const Asp_Event& aspEvent);
        KeyEvent(TWindowId id, KeyEventType type, uint32_t charCode);
        virtual ~KeyEvent();
        void setKeyEventType(KeyEventType type);
        KeyEventType getKeyEventType() const;
        void setCharCode(uint32_t code);
        uint32_t getCharCode() const;
    private:
        KeyEventType _keyEventType;
        uint32_t _charCode;
    };
    
    class Connector
    {
    public:
        Connector();
        virtual ~Connector();
        void subscribe();
        void unsubscribe();
        TWindowId newWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, int rasterType, bool visible);
        void updateWindowSurface(TWindowId id, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height);
        void resizeWindow(TWindowId id, unsigned char *data, unsigned long dataSize, double width, double height);
        void changeWindowVisiblity(TWindowId id, bool visible);
        void bringWindowToFront(TWindowId id);
        void moveWindow(TWindowId id, double x, double y);
        void destroyWindow(TWindowId id);
        std::shared_ptr<Event> waitEvent();
        // Static methods:
        static std::string serverMessageQueueName();
        static std::string messageQueueNameFromPid(TProcId pid);
        static std::string serverSocketPath();
        static std::string sockPathFromPid(TProcId pid);
        // Socket API
        static int listen(const char *sockPath);
        static int accept(int sock);
        static int connect(const char *sockPath);
        static size_t sendAll(int socket, unsigned char *buffer, size_t length, int flags);
        static size_t receiveAll(int sockfd, void *buf, size_t len, int flags);
        static void closeSocket(int sock);
        static void shutdownSocket(int sock);
        
    private:
        TAppId _clientId;
        std::string _msgQueueName;
        std::shared_ptr<message_queue> _msgQueue;
        std::shared_ptr<message_queue> _srvQueue;
        static const int kMsgType;
        int _sock;
        std::vector<TWindowId> _windowIds;
    };
}

#endif
