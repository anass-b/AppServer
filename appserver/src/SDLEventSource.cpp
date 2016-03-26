//
//  GlfwInputSource.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <SDLEventSource.h>
#include <Server.h>
#include <Compositor.h>
#include <SDL.h>

using namespace appserver;

SDLEventSource::SDLEventSource()
{
}

std::shared_ptr<Event> SDLEventSource::pollEvent()
{
    SDL_Event sdlEvent;
    if (SDL_PollEvent(&sdlEvent)) {
        std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();

        if (sdlEvent.type == SDL_QUIT) {
            return std::make_shared<QuitEvent>();
        }
        else if (sdlEvent.type == SDL_MOUSEMOTION) {
            _mousePosX = sdlEvent.motion.x;
            _mousePosY = sdlEvent.motion.y;
            
            std::shared_ptr<MouseMoveEvent> evt = std::make_shared<MouseMoveEvent>();
            evt->setX(sdlEvent.motion.x);
            evt->setY(sdlEvent.motion.y);
            
            return evt;
        }
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP) {
            std::shared_ptr<MouseButtonEvent> evt = std::make_shared<MouseButtonEvent>();
            evt->setX(sdlEvent.motion.x);
            evt->setY(sdlEvent.motion.y);

            if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
                evt->setButtonEventType(AspMouseEventPress);
                _mouseHeld = true;
            }
            else if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
                evt->setButtonEventType(AspMouseEventRelease);
                _mouseHeld = false;
            }
            else {
                evt->setButtonEventType(0);
            }

            if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                evt->setButton(AspMouseButtonRight);
            }
            else if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                evt->setButton(AspMouseButtonLeft);
            }
            else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
                evt->setButton(AspMouseButtonMiddle);
            }
            else {
                evt->setButton(0);
            }
            
            return evt;
        }
        else if (sdlEvent.type == SDL_MOUSEWHEEL) {
            std::shared_ptr<MouseScrollEvent> evt = std::make_shared<MouseScrollEvent>();
            evt->setX(_mousePosX);
            evt->setY(_mousePosY);
            evt->setScrollX(sdlEvent.wheel.x);
            evt->setScrollY(sdlEvent.wheel.y);
            evt->setFlipped(false);
            
            return evt;
        }
        else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP) {
            std::shared_ptr<KeyEvent> evt = std::make_shared<KeyEvent>();
            evt->setKeycode(sdlEvent.key.keysym.sym);
            evt->setKeymod(sdlEvent.key.keysym.mod);
            evt->setScancode(sdlEvent.key.keysym.scancode);
            
            return evt;
        }
        else if (sdlEvent.type == SDL_TEXTINPUT) {
            std::shared_ptr<TextEvent> evt = std::make_shared<TextEvent>();
            evt->setText(sdlEvent.text.text);
            
            return evt;
        }
    }

    return nullptr;
}

