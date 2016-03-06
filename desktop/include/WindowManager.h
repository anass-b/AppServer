//
//  WindowManager.h
//  appserver
//
//  Created by Anass on 10/11/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_WindowManager_h
#define appserver_WindowManager_h

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
        void onKeyEvent(unsigned int charCode);
        
    private:
        Point _mouseDragInitialLocation;
        Window* _windowMoveCandidate;
        Window* _windowWhereMouseDragIsHappening;
    };
}

#endif
