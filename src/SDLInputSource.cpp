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

double SDLInputSource::MousePosX = 0;
double SDLInputSource::MousePosY = 0;
bool SDLInputSource::MouseHeld = false;

SDLInputSource::SDLInputSource(std::shared_ptr<Workspace> screen) : InputSource(screen)
{
}

bool SDLInputSource::pollEvents()
{
    SDL_Event e;
    SDL_PollEvent(&e);
    
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    
    if (e.type == SDL_QUIT) {
        return false;
    }
    else if (e.type == SDL_MOUSEMOTION) {
        this->onMouseMoveEvent(e.motion.x, e.motion.y);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        int aspMouseButton = 0;
        int aspMouseButtonEventType = 0;
        
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            aspMouseButtonEventType = AspMouseEventPress;
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            aspMouseButtonEventType = AspMouseEventRelease;
        }
        
        if (e.button.button == SDL_BUTTON_RIGHT) {
            aspMouseButton = AspMouseButtonRight;
        }
        else if (e.button.button == SDL_BUTTON_LEFT) {
            aspMouseButton = AspMouseButtonLeft;
        }
        else if (e.button.button == SDL_BUTTON_MIDDLE) {
            aspMouseButton = AspMouseButtonMiddle;
        }
        this->onMouseButtonEvent(e.motion.x, e.motion.y, aspMouseButton, aspMouseButtonEventType);
    }
    else if (e.type == SDL_TEXTINPUT) {
        this->onKeyEvent(0);
    }
    
    return true;
}

void SDLInputSource::onMouseMoveEvent(double x, double y)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    if (MouseHeld) {
        wm->onMouseDragEvent(makePoint(x, y));
    }
    else {
        wm->onMouseMoveEvent(makePoint(x, y));
    }
}

void SDLInputSource::onMouseButtonEvent(double x, double y, int button, int type)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    
    if (type == AspMouseEventPress) {
        MouseHeld = true;
    }
    else if (type == AspMouseEventRelease) {
        MouseHeld = false;
    }
    
    wm->onMouseButtonEvent(makePoint(x, y), button, type);
}

void SDLInputSource::onKeyEvent(unsigned int charCode)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    wm->onKeyEvent(charCode);
}

