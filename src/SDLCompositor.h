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

#include <Compositor.h>
#include <SDL.h>

namespace appserver
{
    class SDLCompositor : public Compositor
    {
    public:
        SDLCompositor();
        virtual ~SDLCompositor();
        virtual void compose();
        void setRenderer(SDL_Renderer *renderer);
        SDL_Renderer* getRenderer();
    private:
        SDL_Renderer *_renderer;
    };
}

#endif
