//
//  GLCompositor.c
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <SDLCompositor.h>
#include <memory>

using namespace appserver;
 
SDLCompositor::SDLCompositor()
{
}

void SDLCompositor::compose()
{
    SDL_SetRenderDrawColor(_renderer, 65, 105, 170, 255);
    SDL_RenderClear(_renderer);
    
    for (auto &it : _windows) {
        std::shared_ptr<Window> w(it);
        if (w != nullptr) {
            w->performOperationsAndDraw();
        }
    }
    
    SDL_RenderPresent(_renderer);
}

void SDLCompositor::setRenderer(SDL_Renderer *renderer)
{
    _renderer = renderer;
}

SDL_Renderer* SDLCompositor::getRenderer()
{
    return _renderer;
}

SDLCompositor::~SDLCompositor()
{
}

