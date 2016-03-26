#ifndef EVENTS_H
#define EVENTS_H

namespace appserver {

enum EventType
{
    kEventTypeUnknown,
    kEventTypeQuit,
    kEventTypeMouseMove,
    kEventTypeMouseButton,
    kEventTypeMouseScroll,
    kEventTypeText,
    kEventTypeKey
};

class Event
{
public:
    Event(EventType type) : _type(type) {}
    virtual ~Event() {}
    int32_t getTimestamp() const { return _timestamp; }
    void setTimestamp(int32_t timestamp) { _timestamp = timestamp; }
    EventType getType() const { return _type; };
    void setType(EventType type) { _type = type; }
private:
    int32_t _timestamp;
    EventType _type;
};

class QuitEvent : public Event
{
public:
    QuitEvent() : Event(kEventTypeQuit) {}
    virtual ~QuitEvent() {}
};

class MouseEvent : public Event
{
public:
    MouseEvent(EventType type) : Event(type) {}
    virtual ~MouseEvent()  {}
    double getX() const { return _x; };
    void setX(double x) { _x = x; }
    double getY() const { return _y; };
    void setY(double y) { _y = y; }
private:
    double _x = 0.0f;
    double _y = 0.0f;
};

class MouseMoveEvent : public MouseEvent
{
public:
    MouseMoveEvent() : MouseEvent(kEventTypeMouseMove) {}
    virtual ~MouseMoveEvent() {}
};

class MouseButtonEvent : public MouseEvent
{
public:
    MouseButtonEvent() : MouseEvent(kEventTypeMouseButton) {}
    virtual ~MouseButtonEvent() {}
    int8_t getButton() const { return _button; };
    void setButton(int8_t button) { _button = button; }
    int8_t getButtonEventType() const { return _buttonEventType; };
    void setButtonEventType(int8_t buttonEventType) { _buttonEventType = buttonEventType; }
private:
    int8_t _button;
    int8_t _buttonEventType;
};

class MouseScrollEvent : public MouseEvent
{
public:
    MouseScrollEvent() : MouseEvent(kEventTypeMouseScroll) {}
    virtual ~MouseScrollEvent() {}
    int8_t getScrollX() const { return _scrollX; };
    void setScrollX(int8_t scrollX) { _scrollX = scrollX; }
    int8_t getScrollY() const { return _scrollY; };
    void setScrollY(int8_t scrollY) { _scrollY = scrollY; }
    bool getFlipped() const { return _flipped; };
    void setFlipped(bool flipped) { _flipped = flipped; }
private:
    int32_t _scrollX;
    int32_t _scrollY;
    bool _flipped;
};

class TextEvent : public Event
{
public:
    TextEvent() : Event(kEventTypeText) {}
    virtual ~TextEvent() {}
    const std::string& getText() const { return _text; };
    void setText(const std::string text) { _text = text; }
private:
    std::string _text;
};

class KeyEvent : public Event
{
public:
    KeyEvent() : Event(kEventTypeKey) {}
    virtual ~KeyEvent() {}
    int32_t getScancode() const { return _scancode; };
    void setScancode(int32_t scancode) { _scancode = scancode; }
    int32_t getKeycode() const { return _keycode; };
    void setKeycode(int32_t keycode) { _keycode = keycode; }
    int32_t getKeymod() const { return _keymod; };
    void setKeymod(int32_t keymod) { _keymod = keymod; }
    int8_t getState() const { return _state; };
    void setState(int8_t state) { _state = state; }
    bool getRepeat() const { return _repeat; };
    void setRepeat(bool repeat) { _repeat = repeat; }
private:
    int32_t _scancode;
    int32_t _keycode;
    int32_t _keymod;
    int8_t _state;
    bool _repeat;
};
    
}

#endif