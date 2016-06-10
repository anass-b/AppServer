//
//  GlfwInputSource.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <sdleventsource.h>
#include <server.h>
#include <compositor.h>
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
            evt->setTimestamp(sdlEvent.motion.timestamp);
            evt->setX(sdlEvent.motion.x);
            evt->setY(sdlEvent.motion.y);

            return evt;
        }
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP) {
            std::shared_ptr<MouseButtonEvent> evt = std::make_shared<MouseButtonEvent>();
            evt->setTimestamp(sdlEvent.button.timestamp);
            evt->setX(sdlEvent.motion.x);
            evt->setY(sdlEvent.motion.y);

            if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
                evt->setState(AspMouseButtonStatePressed);
                _mouseHeld = true;
            }
            else if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
                evt->setState(AspMouseButtonStateReleased);
                _mouseHeld = false;
            }
            else {
                evt->setState(0);
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
                evt->setButton(AspMouseButtonUndefined);
            }

            return evt;
        }
        else if (sdlEvent.type == SDL_MOUSEWHEEL) {
            std::shared_ptr<MouseScrollEvent> evt = std::make_shared<MouseScrollEvent>();
            evt->setTimestamp(sdlEvent.wheel.timestamp);
            evt->setX(_mousePosX);
            evt->setY(_mousePosY);
            evt->setScrollX(sdlEvent.wheel.x);
            evt->setScrollY(sdlEvent.wheel.y);
            evt->setFlipped(false);

            return evt;
        }
        else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP) {
            std::shared_ptr<KeyEvent> evt = std::make_shared<KeyEvent>();
            evt->setTimestamp(sdlEvent.key.timestamp);
            evt->setKeycode(sdlEvent.key.keysym.sym);
            evt->setKeymod(sdlEvent.key.keysym.mod);
            evt->setScancode(sdlEvent.key.keysym.scancode);
            evt->setRepeat(sdlEvent.key.repeat);

            if (sdlEvent.key.state == SDL_PRESSED) {
                evt->setState(AspKeyStatePressed);
            }
            else if (sdlEvent.key.state == SDL_RELEASED) {
                evt->setState(AspKeyStateReleased);
            }
            else {
                evt->setState(AspKeyStateUndefined);
            }

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
