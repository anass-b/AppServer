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
#include <Window.h>
#include <Geometry.h>
#include <Events.h>

namespace appserver
{
    class Window;
    
    class WindowManager
    {
    public:
        WindowManager();
        bool sendEvent(std::shared_ptr<Event> evt);
    private:
        bool sendMouseMoveEvent(std::shared_ptr<MouseMoveEvent> evt);
        bool sendMouseButtonEvent(std::shared_ptr<MouseButtonEvent> evt);
        bool sendMouseScrollEvent(std::shared_ptr<MouseScrollEvent> evt);
        bool sendTextEvent(std::shared_ptr<TextEvent> evt);
        bool sendKeyEvent(std::shared_ptr<KeyEvent> evt);    
    };
}

#endif
