//
//  GlfwScreen.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef SDLWORKSPACE_H
#define SDLWORKSPACE_H

#include <iostream>
#include <SDL.h>
#include <Workspace.h>

namespace appserver
{
    class SDLWorkspace : public Workspace, public std::enable_shared_from_this<SDLWorkspace>
    {
    public:
        friend class SDLInputSource;
        SDLWorkspace(std::shared_ptr<Compositor> compositor, unsigned int width = 1600, unsigned int height = 1000);
        virtual ~SDLWorkspace();
        virtual void run();
    private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;
    };
}

#endif
