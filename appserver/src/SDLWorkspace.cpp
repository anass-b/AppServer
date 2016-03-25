//
//  GlfwScreen.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <SDLWorkspace.h>
#include <SDLInputSource.h>
#include <SDLCompositor.h>

using namespace appserver;
 
SDLWorkspace::SDLWorkspace(std::shared_ptr<Compositor> compositor, unsigned int width, unsigned int height) : Workspace(compositor)
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height cannot be negative or zero");
    }
    
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        _window = SDL_CreateWindow("appserver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
        
        _compositor->setSize(makeSize(width, height));
        std::shared_ptr<SDLCompositor> sdlCompositor = std::dynamic_pointer_cast<SDLCompositor>(_compositor);
        sdlCompositor->setRenderer(_renderer);
    }
}

void SDLWorkspace::run()
{
    _inputSource = std::make_shared<SDLInputSource>(shared_from_this());
    std::shared_ptr<SDLInputSource> inputSource = std::dynamic_pointer_cast<SDLInputSource>(_inputSource);
    
    while (true) {
        _compositor->compose();

        if(!inputSource->pollEvents()) {
            SDL_Quit();
            return;
        }

        Workspace::avoidBusyWait(10 * NANO_SECOND_MULTIPLIER);
    }
}

SDLWorkspace::~SDLWorkspace()
{
    SDL_Quit();
}

