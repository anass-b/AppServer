//
//  WindowManager.cpp
//  appserver
//
//  Created by Anass on 10/11/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <windowmanager.h>
#include <server.h>
#include <compositor.h>
#include <window.h>
#include <app.h>

using namespace appserver;

WindowManager::WindowManager()
{
}

bool WindowManager::sendEvent(std::shared_ptr<Event> evt)
{
    uint8_t eventType = evt->getType();
    
    if (eventType == AspEventTypeKey) {
        std::shared_ptr<KeyEvent> keyEvent = std::dynamic_pointer_cast<KeyEvent>(evt);
        return this->sendKeyEvent(keyEvent);
    }
    else if (eventType == AspEventTypeText) {
        std::shared_ptr<TextEvent> textEvent = std::dynamic_pointer_cast<TextEvent>(evt);
        return this->sendTextEvent(textEvent);
    }
    else if (eventType == AspEventTypeMouseScroll) {
        std::shared_ptr<MouseScrollEvent> mouseScrollEvent = std::dynamic_pointer_cast<MouseScrollEvent>(evt);
        return this->sendMouseScrollEvent(mouseScrollEvent);
    }
    else if (eventType == AspEventTypeMouseButton) {
        std::shared_ptr<MouseButtonEvent> mouseButtonEvent = std::dynamic_pointer_cast<MouseButtonEvent>(evt);
        return this->sendMouseButtonEvent(mouseButtonEvent);
    }
    else if (eventType == AspEventTypeMouseMove) {
        std::shared_ptr<MouseMoveEvent> mouseMoveEvent = std::dynamic_pointer_cast<MouseMoveEvent>(evt);
        return this->sendMouseMoveEvent(mouseMoveEvent);
    }
    return false;
}

bool WindowManager::sendMouseMoveEvent(std::shared_ptr<MouseMoveEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    Point mouseLocation = makePoint(evt->getX(), evt->getY());
    std::shared_ptr<Window> window = _focusedWindow != nullptr ? _focusedWindow : compositor->findWindowInLocation(mouseLocation);
    if (window != nullptr) {
        std::shared_ptr<App> app = window->getApp().lock();
        Point locationInWindow = window->getLocationInWindow(makePoint(evt->getX(), evt->getY()));
        evt->setWindowX(locationInWindow.x);
        evt->setWindowY(locationInWindow.y);
        app->sendMouseMoveEvent(evt, window);
        return true;
    }
    
    return false;
}

bool WindowManager::sendMouseButtonEvent(std::shared_ptr<MouseButtonEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    Point mouseLocation = makePoint(evt->getX(), evt->getY());    
    std::shared_ptr<Window> window = nullptr;
    if (evt->getState() == AspMouseButtonStateReleased && _focusedWindow != nullptr) {
        window = _focusedWindow;
    }
    else {
        window = compositor->findWindowInLocation(mouseLocation);
    }

    if (window != nullptr) {
        compositor->bringWindowToFront(window);

        if (evt->getState() == AspMouseButtonStatePressed) {
            _focusedWindow = window;
        }
        if (evt->getState() == AspMouseButtonStateReleased) {
            _focusedWindow = nullptr;
        }

        std::shared_ptr<App> app = window->getApp().lock();
        Point locationInWindow = window->getLocationInWindow(makePoint(evt->getX(), evt->getY()));
        evt->setWindowX(locationInWindow.x);
        evt->setWindowY(locationInWindow.y);
        app->sendMouseButtonEvent(evt, window);
    }
    
    return false;
}

bool WindowManager::sendMouseScrollEvent(std::shared_ptr<MouseScrollEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    Point mouseLocation = makePoint(evt->getX(), evt->getY());
    std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
    if (window) {
        std::shared_ptr<App> app = window->getApp().lock();
        Point locationInWindow = window->getLocationInWindow(makePoint(evt->getX(), evt->getY()));
        evt->setWindowX(locationInWindow.x);
        evt->setWindowY(locationInWindow.y);
        app->sendMouseScrollEvent(evt, window);
    }
    return false;
}

bool WindowManager::sendTextEvent(std::shared_ptr<TextEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    std::shared_ptr<Window> topMostWindow = compositor->getTopMostWindow();
    if (topMostWindow != nullptr) {
        std::shared_ptr<App> app = topMostWindow->getApp().lock();
        if (app != nullptr) {
            app->sendTextEvent(evt, topMostWindow);
        }
    }
    return false;
}

bool WindowManager::sendKeyEvent(std::shared_ptr<KeyEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    std::shared_ptr<Window> topMostWindow = compositor->getTopMostWindow();
    if (topMostWindow != nullptr) {
        std::shared_ptr<App> app = topMostWindow->getApp().lock();
        if (app != nullptr) {
            app->sendKeyEvent(evt, topMostWindow);
        }
    }
    return false;
}


