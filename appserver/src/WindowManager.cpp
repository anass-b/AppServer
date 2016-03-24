//
//  WindowManager.cpp
//  appserver
//
//  Created by Anass on 10/11/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <WindowManager.h>
#include <Server.h>
#include <Compositor.h>
#include <Window.h>
#include <App.h>

using namespace appserver;

WindowManager::WindowManager() : _windowWhereMouseDragIsHappening(nullptr)
{
}

void WindowManager::onMouseMoveEvent(Point mouseLocation)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    
    std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
    if (window != nullptr) {
        std::shared_ptr<App> app = window->getApp().lock();
        Point locationInWindow = window->getLocationInWindow(mouseLocation);
        app->sendMouseMoveEvent(window->getId(), AspMouseEventMove, locationInWindow.x, locationInWindow.y, mouseLocation.x, mouseLocation.y);
    }
}

void WindowManager::onMouseDragEvent(Point mouseLocation)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    
    if (_windowWhereMouseDragIsHappening) {
        std::shared_ptr<App> app = _windowWhereMouseDragIsHappening->getApp().lock();
        Point locationInWindow = _windowWhereMouseDragIsHappening->getLocationInWindow(mouseLocation);
        app->sendMouseMoveEvent(_windowWhereMouseDragIsHappening->getId(), AspMouseEventDrag, locationInWindow.x, locationInWindow.y, mouseLocation.x, mouseLocation.y);
    }
}

void WindowManager::onMouseButtonEvent(Point mouseLocation, int button, int type)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    
    if (type == AspMouseEventPress) {
        std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
        if (window != nullptr) {
            compositor->bringWindowToFront(window);
            _windowWhereMouseDragIsHappening = window.get();
            std::shared_ptr<App> app = window->getApp().lock();
            Point windowLocation = window->getFrame().location;
            app->sendMouseButtonEvent(window->getId(), type, button, mouseLocation.x - windowLocation.x, mouseLocation.y - windowLocation.y, mouseLocation.x, mouseLocation.y);
        }
    }
    else if (type == AspMouseEventRelease) {
        if (_windowWhereMouseDragIsHappening /*window where the mouse button press happened*/) {
            std::shared_ptr<App> app = _windowWhereMouseDragIsHappening->getApp().lock();
            Point windowLocation = _windowWhereMouseDragIsHappening->getFrame().location;
            app->sendMouseButtonEvent(_windowWhereMouseDragIsHappening->getId(), type, button, mouseLocation.x - windowLocation.x, mouseLocation.y - windowLocation.y, mouseLocation.x, mouseLocation.y);
        }
        else {
            std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
            if (window) {
                std::shared_ptr<App> app = window->getApp().lock();
                Point windowLocation = window->getFrame().location;
                app->sendMouseButtonEvent(window->getId(), type, button, mouseLocation.x - windowLocation.x, mouseLocation.y - windowLocation.y, mouseLocation.x, mouseLocation.y);
            }            
        }
        
        _windowWhereMouseDragIsHappening = nullptr;
    }
}


void WindowManager::onTextEvent(std::string text)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    std::shared_ptr<Window> topMostWindow = compositor->getTopMostWindow();
    if (topMostWindow != nullptr) {
        std::shared_ptr<App> app = topMostWindow->getApp().lock();
        if (app != nullptr) {
            app->sendTextEvent(topMostWindow->getId(), text);
        }
    }
}




