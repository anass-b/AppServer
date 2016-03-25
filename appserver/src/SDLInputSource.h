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
        void onMouseMoveEvent(const MouseMoveEvent &evt);
        void onMouseButtonEvent(const MouseButtonEvent &evt);
        void onMouseWheelEvent(const MouseWheelEvent &evt);
        void onTextEvent(const TextEvent &evt);
        void onKeyEvent(const KeyEvent &evt);
    private:
        double _mousePosX = 0;
        double _mousePosY = 0;
        bool _mouseHeld = false;
    };
}

#endif
