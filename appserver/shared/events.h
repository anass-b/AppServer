#ifndef EVENTS_H
#define EVENTS_H

#include <protocol.h>

namespace appserver {

class Event
{
public:
    Event(uint8_t type) : _type(type) {}

    Event(const Asp_Event &evt)
    {
        setWindowId(evt.winId);
        setTimestamp(evt.timestamp);
        setType(evt.type);
    }

    virtual ~Event() {}
    uint32_t getTimestamp() const { return _timestamp; }
    void setTimestamp(uint32_t timestamp) { _timestamp = timestamp; }
    uint8_t getType() const { return _type; }
    void setType(uint8_t type) { _type = type; }
    TWindowId getWindowId() const { return _windowId; }
    void setWindowId(TWindowId windowId) { _windowId = windowId; }

    virtual Asp_Event toProtocolEvent()
    {
        Asp_Event evt;
        evt.winId = getWindowId();
        evt.type = getType();
        evt.field0 = 0;
        evt.field1 = 0;
        evt.field3 = 0;
        evt.field4 = 0;
        evt.field2 = 0;
        evt.field3 = 0;
        evt.field4 = 0;
        evt.field5 = 0;
        evt.timestamp = getTimestamp();
        
        return evt;
    }
private:
    uint8_t _type = AspEventTypeUndefined;
    TWindowId _windowId = AspUndefinedWindowId;
    uint32_t _timestamp = 0;
};

class QuitEvent : public Event
{
public:
    QuitEvent() : Event(AspEventTypeQuit) {}
    virtual ~QuitEvent() {}
};

class MouseEvent : public Event
{
public:
    MouseEvent(const Asp_Event &evt): Event(evt) {}
    MouseEvent(uint8_t type) : Event(type) {}
    virtual ~MouseEvent()  {}
    double getX() const { return _x; }
    void setX(double x) { _x = x; }
    double getY() const { return _y; }
    void setY(double y) { _y = y; }
    double getWindowX() const { return _windowX; }
    void setWindowX(double windowX) { _windowX = windowX; }
    double getWindowY() const { return _windowY; }
    void setWindowY(double windowY) { _windowY = windowY; }
private:
    double _x = 0.0f;
    double _y = 0.0f;
    double _windowX = 0.0f;
    double _windowY = 0.0f;
};

class MouseMoveEvent : public MouseEvent
{
public:
    MouseMoveEvent() : MouseEvent(AspEventTypeMouseMove) {}

    MouseMoveEvent(const Asp_Event &evt): MouseEvent(evt)
    {
        setWindowX(evt.field0);
        setWindowY(evt.field1);
        setX(evt.field2);
        setY(evt.field3);
    }

    virtual ~MouseMoveEvent() {}

    virtual Asp_Event toProtocolEvent()
    {
        Asp_Event evt;
        evt.winId = getWindowId();
        evt.type = getType();
        evt.field0 = getWindowX();
        evt.field1 = getWindowY();
        evt.field2 = getX();
        evt.field3 = getY();
        evt.field4 = 0;
        evt.field5 = 0;
        evt.timestamp = getTimestamp();

        return evt;
    }
};

class MouseButtonEvent : public MouseEvent
{
public:
    MouseButtonEvent() : MouseEvent(AspEventTypeMouseButton) {}

    MouseButtonEvent(const Asp_Event &evt) : MouseEvent(evt)
    {
        setWindowX(evt.field0);
        setWindowY(evt.field1);
        setX(evt.field2);
        setY(evt.field3);
        setButton(evt.field4);
        setState(evt.field5);
    }

    virtual ~MouseButtonEvent() {}
    uint8_t getButton() const { return _button; }
    void setButton(uint8_t button) { _button = button; }
    uint8_t getState() const { return _state; }
    void setState(uint8_t state) { _state = state; }

    virtual Asp_Event toProtocolEvent()
    {
        Asp_Event evt;
        evt.winId = getWindowId();
        evt.type = getType();
        evt.field0 = getWindowX();
        evt.field1 = getWindowY();
        evt.field2 = getX();
        evt.field3 = getY();
        evt.field4 = getButton();
        evt.field5 = getState();
        evt.timestamp = getTimestamp();
        
        return evt;
    }
private:
    uint8_t _button;
    uint8_t _state;
};

class MouseScrollEvent : public MouseEvent
{
public:
    MouseScrollEvent() : MouseEvent(AspEventTypeMouseScroll) {}

    MouseScrollEvent(const Asp_Event &evt) : MouseEvent(evt)
    {
        setWindowX(evt.field0);
        setWindowY(evt.field1);
        setScrollX(evt.field2);
        setScrollY(evt.field3);
    }

    virtual ~MouseScrollEvent() {}
    int32_t getScrollX() const { return _scrollX; }
    void setScrollX(int32_t scrollX) { _scrollX = scrollX; }
    int32_t getScrollY() const { return _scrollY; }
    void setScrollY(int32_t scrollY) { _scrollY = scrollY; }
    bool getFlipped() const { return _flipped; }
    void setFlipped(bool flipped) { _flipped = flipped; }

    virtual Asp_Event toProtocolEvent()
    {
        Asp_Event evt;
        evt.winId = getWindowId();
        evt.type = getType();
        evt.field0 = getWindowX();
        evt.field1 = getWindowY();
        evt.field2 = getScrollX();
        evt.field3 = getScrollY();
        evt.field4 = 0;
        evt.field5 = 0;
        evt.timestamp = getTimestamp();

        return evt;
    }
private:
    int32_t _scrollX;
    int32_t _scrollY;
    bool _flipped;
};

class TextEvent : public Event
{
public:
    TextEvent() : Event(AspEventTypeText) {}
    virtual ~TextEvent() {}
    const std::string& getText() const { return _text; }
    void setText(const std::string text) { _text = text; }

    virtual Asp_Event toProtocolEvent()
    {
        Asp_Event evt;
        evt.winId = getWindowId();
        evt.type = getType();
        evt.field0 = 0;
        evt.field1 = 0;
        evt.field2 = 0;
        evt.field3 = 0;
        evt.field4 = 0;
        evt.field5 = getText().size();
        evt.timestamp = getTimestamp();

        return evt;
    }
private:
    std::string _text;
};

class KeyEvent : public Event
{
public:
    KeyEvent() : Event(AspEventTypeKey) {}
    KeyEvent(const Asp_Event &evt) : Event(evt)
    {
        setKeycode(evt.field0);
        setScancode(evt.field1);
        setKeymod(evt.field2);
        setState(evt.field3);
        setRepeat(evt.field4);
    }
    virtual ~KeyEvent() {}
    int32_t getScancode() const { return _scancode; }
    void setScancode(int32_t scancode) { _scancode = scancode; }
    int32_t getKeycode() const { return _keycode; }
    void setKeycode(int32_t keycode) { _keycode = keycode; }
    int32_t getKeymod() const { return _keymod; }
    void setKeymod(int32_t keymod) { _keymod = keymod; }
    int8_t getState() const { return _state; }
    void setState(int8_t state) { _state = state; }
    bool getRepeat() const { return _repeat; }
    void setRepeat(bool repeat) { _repeat = repeat; }

    virtual Asp_Event toProtocolEvent()
    {
        Asp_Event evt;
        evt.winId = getWindowId();
        evt.type = getType();
        evt.field0 = getKeycode();
        evt.field1 = getScancode();
        evt.field2 = getKeymod();
        evt.field3 = getState();
        evt.field4 = getRepeat();
        evt.field5 = 0;
        evt.timestamp = getTimestamp();

        return evt;
    }
private:
    int32_t _scancode;
    int32_t _keycode;
    int32_t _keymod;
    int8_t _state;
    bool _repeat;
};
    
}

#endif
