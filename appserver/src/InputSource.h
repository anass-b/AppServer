//
//  InputSource.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef INPUTSOURCE_H
#define INPUTSOURCE_H

#include <iostream>
#include <memory>

namespace appserver
{
    class Workspace;
    
    class InputEvent
    {
    public:
        InputEvent() {}
        int32_t getTimestamp() const { return _timestamp; };
        void setTimestamp(int32_t timestamp) { _timestamp = timestamp; }
    private:
        int32_t _timestamp;
    };
    
    class MouseEvent : public InputEvent
    {
    public:
        MouseEvent() {}
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
        MouseMoveEvent() {}
    };
    
    class MouseButtonEvent : public MouseEvent
    {
    public:
        MouseButtonEvent() {}
        int8_t getButton() const { return _button; };
        void setButton(int8_t button) { _button = button; }
        int8_t getType() const { return _type; };
        void setType(int8_t type) { _type = type; }
    private:
        int8_t _button;
        int8_t _type;
    };
    
    class MouseWheelEvent : public MouseEvent
    {
    public:
        MouseWheelEvent() {}
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
    
    class TextEvent : public InputEvent
    {
    public:
        TextEvent() {}
        const std::string& getText() const { return _text; };
        void setText(const std::string text) { _text = text; }
    private:
        std::string _text;
    };
    
    class KeyEvent : public InputEvent
    {
    public:
        KeyEvent() {}
        int32_t getScancode() const { return _scancode; };
        void setScancode(int32_t scancode) { _scancode = scancode; }
        int32_t getKeycode() const { return _keycode; };
        void setKeycode(int32_t keycode) { _keycode = keycode; }
        int32_t getKeymod() const { return _keymod; };
        void setKeymod(int32_t keymod) { _keymod = keymod; }
        int8_t getState() const { return _state; };
        void setKeymod(int8_t state) { _state = state; }
        bool getRepeat() const { return _repeat; };
        void setRepeat(bool repeat) { _repeat = repeat; }
    private:
        int32_t _scancode;
        int32_t _keycode;
        int32_t _keymod;
        int8_t _state;
        bool _repeat;
    };
    
    class InputSource
    {
    public:
        InputSource();
        virtual bool pollEvents() = 0;
    };
}

#endif
