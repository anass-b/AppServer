//
//  GLCompositor.c
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <sdlcompositor.h>
#include <memory>

using namespace appserver;
 
SDLCompositor::SDLCompositor()
{
    int width = 1920;
    int height = 1080;
    this->setSize(makeSize(width, height));
    
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        _window = SDL_CreateWindow("appserver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    }
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

SDL_Renderer* SDLCompositor::getRenderer() const
{
    return _renderer;
}

SDLCompositor::~SDLCompositor()
{
    SDL_Quit();
}

