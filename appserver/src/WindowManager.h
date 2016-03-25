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
#include <Geometry.h>

namespace appserver
{
    class Window;
    
    class WindowManager
    {
    public:
        WindowManager();
        void onMouseMoveEvent(Point mouseLocation);
        void onMouseDragEvent(Point mouseLocation);
        void onMouseButtonEvent(Point mouseLocation, int button, int type);
        void onMouseWheelEvent(Point mouseLocation, int scrollX, int scrollY, bool flipped);
        void onTextEvent(std::string text);
        void onKeyEvent(int key);
        
    private:
        Window* _windowWhereMouseDragIsHappening;
    };
}

#endif
