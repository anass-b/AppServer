//
//  GlfwInputSource.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef SDLINPUTSOURCE_H
#define SDLINPUTSOURCE_H

#include <memory>
#include <GLFW/glfw3.h>
#include <InputSource.h>

namespace appserver
{
    class SDLInputSource : public InputSource
    {
    public:
        SDLInputSource(std::shared_ptr<Workspace> screen);
        virtual bool pollEvents();
        virtual void onMouseMoveEvent(double x, double y);
        virtual void onMouseButtonEvent(double x, double y, int button, int type);
        virtual void onKeyEvent(unsigned int charCode);
    private:
        static double MousePosX;
        static double MousePosY;
        static bool MouseHeld;
    };
}

#endif
