//
//  GLCompositor.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef SDLCOMPOSITOR_H
#define SDLCOMPOSITOR_H

#include <memory>

#include <compositor.h>
#include <SDL.h>

namespace appserver
{
    class SDLCompositor : public Compositor
    {
    public:
        SDLCompositor();
        virtual ~SDLCompositor();
        virtual void compose();
        SDL_Renderer* getRenderer() const;
    private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;
    };
}

#endif
