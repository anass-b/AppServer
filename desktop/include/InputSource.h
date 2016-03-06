//
//  InputSource.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_InputSource_h
#define appserver_InputSource_h

#include <iostream>

namespace appserver
{
    class Workspace;
    
    class InputSource
    {
    public:
        InputSource(std::shared_ptr<Workspace> screen);
        virtual void pollEvents() = 0;
        virtual void onMouseMoveEvent(double x, double y) = 0;
        virtual void onMouseButtonEvent(double x, double y, int button, int type) = 0;
        virtual void onKeyEvent(unsigned int charCode) = 0;
    protected:
        std::shared_ptr<Workspace> _workspace;
    };
}

#endif
