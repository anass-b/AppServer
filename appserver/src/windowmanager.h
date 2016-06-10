//
//  WindowManager.h
//  appserver
//
//  Created by Anass on 10/11/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <iostream>
#include <window.h>
#include <geometry.h>
#include <events.h>

namespace appserver {
class Window;

class WindowManager {
public:
    WindowManager();
    bool sendEvent(std::shared_ptr<Event> evt);

private:
    bool sendMouseMoveEvent(std::shared_ptr<MouseMoveEvent> evt);
    bool sendMouseButtonEvent(std::shared_ptr<MouseButtonEvent> evt);
    bool sendMouseScrollEvent(std::shared_ptr<MouseScrollEvent> evt);
    bool sendTextEvent(std::shared_ptr<TextEvent> evt);
    bool sendKeyEvent(std::shared_ptr<KeyEvent> evt);

private:
    std::shared_ptr<Window> _focusedWindow = nullptr;
};
}

#endif
