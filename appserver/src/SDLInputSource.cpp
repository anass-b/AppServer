//
//  GlfwInputSource.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <SDLInputSource.h>
#include <SDLWorkspace.h>
#include <Server.h>
#include <Compositor.h>

using namespace appserver;

SDLInputSource::SDLInputSource(std::shared_ptr<Workspace> screen) : InputSource(screen)
{
}

bool SDLInputSource::pollEvents()
{
    SDL_Event sdlEvent;
    if (SDL_PollEvent(&sdlEvent)) {
        std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();

        if (sdlEvent.type == SDL_QUIT) {
            return false;
        }
        else if (sdlEvent.type == SDL_MOUSEMOTION) {
            _mousePosX = sdlEvent.motion.x;
            _mousePosY = sdlEvent.motion.y;
            
            MouseMoveEvent evt;
            evt.setX(sdlEvent.motion.x);
            evt.setY(sdlEvent.motion.y);
            
            this->onMouseMoveEvent(evt);
        }
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP) {
            MouseButtonEvent evt;
            evt.setX(sdlEvent.motion.x);
            evt.setY(sdlEvent.motion.y);

            if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
                evt.setType(AspMouseEventPress);
            }
            else if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
                evt.setType(AspMouseEventRelease);
            }
            else {
                evt.setType(0);
            }

            if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                evt.setButton(AspMouseButtonRight);
            }
            else if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                evt.setButton(AspMouseButtonLeft);
            }
            else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
                evt.setButton(AspMouseButtonMiddle);
            }
            else {
                evt.setButton(0);
            }
            this->onMouseButtonEvent(evt);
        }
        else if (sdlEvent.type == SDL_MOUSEWHEEL) {
            MouseWheelEvent evt;
            evt.setX(_mousePosX);
            evt.setY(_mousePosY);
            evt.setScrollX(sdlEvent.wheel.x);
            evt.setScrollY(sdlEvent.wheel.y);
            evt.setFlipped(false);
            
            this->onMouseWheelEvent(evt);
        }
        else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP) {
            KeyEvent evt;
            if (sdlEvent.key.keysym.sym == SDLK_RETURN) {
                //this->onKeyEvent(AspKeyScancodeReturn);
            }
            else if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE) {
                //this->onKeyEvent(AspKeyScancodeBackspace);SDL_SCANCODE_BACKSPACE
            }
        }
        else if (sdlEvent.type == SDL_TEXTINPUT) {
            TextEvent evt;
            evt.setText(sdlEvent.text.text);
            
            this->onTextEvent(evt);
        }
    }

    return true;
}

void SDLInputSource::onMouseMoveEvent(const MouseMoveEvent &evt)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    if (_mouseHeld) {
        wm->onMouseDragEvent(makePoint(evt.getX(), evt.getY()));
    }
    else {
        wm->onMouseMoveEvent(makePoint(evt.getX(), evt.getY()));
    }
}

void SDLInputSource::onMouseButtonEvent(const MouseButtonEvent &evt)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    
    if (evt.getType() == AspMouseEventPress) {
        _mouseHeld = true;
    }
    else if (evt.getType() == AspMouseEventRelease) {
        _mouseHeld = false;
    }
    
    wm->onMouseButtonEvent(makePoint(evt.getX(), evt.getY()), evt.getButton(), evt.getType());
}

void SDLInputSource::onMouseWheelEvent(const MouseWheelEvent &evt)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    wm->onMouseWheelEvent(makePoint(evt.getX(), evt.getY()), evt.getScrollX(), evt.getScrollY(), evt.getFlipped());
}

void SDLInputSource::onTextEvent(const TextEvent &evt)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    wm->onTextEvent(evt.getText());
}

void SDLInputSource::onKeyEvent(const KeyEvent &evt)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    wm->onKeyEvent(evt.getKeycode());
}

