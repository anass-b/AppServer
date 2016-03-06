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
#include <HWindow.h>
#include <App.h>

using namespace appserver;

WindowManager::WindowManager() : _windowMoveCandidate(nullptr), _mouseDragInitialLocation(makePoint(0.0, 0.0)), _windowWhereMouseDragIsHappening(nullptr)
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
    
    if (_windowMoveCandidate) {
        // We move the window and send the update to it's owner (client app)
        Point newLocation = makePoint(mouseLocation.x - _mouseDragInitialLocation.x, mouseLocation.y - _mouseDragInitialLocation.y);
        _windowMoveCandidate->setLocation(newLocation);
        _windowMoveCandidate->move(newLocation);
        Point newlocation = _windowMoveCandidate->getFrame().location;
        std::shared_ptr<App> app = _windowMoveCandidate->getApp().lock();
        if (app != nullptr) {
            app->sendMouseMoveEvent(_windowMoveCandidate->getId(), -1, newlocation.x, newlocation.y, -1, -1);
        }
    }
    else if (_windowWhereMouseDragIsHappening) {
        std::shared_ptr<App> app = _windowWhereMouseDragIsHappening->getApp().lock();
        Point locationInWindow = _windowWhereMouseDragIsHappening->getLocationInWindow(mouseLocation);
        app->sendMouseMoveEvent(_windowWhereMouseDragIsHappening->getId(), AspMouseEventDrag, locationInWindow.x, locationInWindow.y, mouseLocation.x, mouseLocation.y);
    }
    else {
        std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
        if (window != nullptr) {
            std::shared_ptr<App> app = window->getApp().lock();
            if (app != nullptr) {
                Point locationInWindow = window->getLocationInWindow(mouseLocation);
                app->sendMouseMoveEvent(window->getId(), AspMouseEventDrag, locationInWindow.x, locationInWindow.y, mouseLocation.x, mouseLocation.y);
            }
        }
    }
}

void WindowManager::onMouseButtonEvent(Point mouseLocation, int button, int type)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    
    if (type == AspMouseEventPress) {
        std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
        if (window != nullptr) {
            compositor->bringWindowToFront(window);
            if (window->pointIsInsideTitleBar(mouseLocation)) {
                _mouseDragInitialLocation = makePoint(mouseLocation.x - window->getX(), mouseLocation.y - window->getY());
                _windowMoveCandidate = window.get();
            }
            else {
                _windowWhereMouseDragIsHappening = window.get();
            }
        }
    }
    else if (type == AspMouseEventRelease) {
        _mouseDragInitialLocation = makePoint(0.0, 0.0);
        _windowMoveCandidate = nullptr;
        _windowWhereMouseDragIsHappening = nullptr;
    }
    
    std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
    if (window != nullptr) {
        std::shared_ptr<App> app = window->getApp().lock();
        Point windowLocation = window->getFrame().location;
        app->sendMouseButtonEvent(window->getId(), type, button, mouseLocation.x - windowLocation.x, mouseLocation.y - windowLocation.y, mouseLocation.x, mouseLocation.y);
    }
}


void WindowManager::onKeyEvent(unsigned int charCode)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    std::shared_ptr<Window> topMostWindow = compositor->getTopMostWindow();
    if (topMostWindow != nullptr) {
        std::shared_ptr<App> app = topMostWindow->getApp().lock();
        if (app != nullptr) {
            app->sendKeyEvent(topMostWindow->getId(), charCode);
        }
    }
}




