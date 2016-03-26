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

WindowManager::WindowManager()
{
}

bool WindowManager::sendEvent(std::shared_ptr<Event> evt)
{
    EventType eventType = evt->getType();
    
    if (eventType == kEventTypeKey) {
        std::shared_ptr<KeyEvent> keyEvent = std::dynamic_pointer_cast<KeyEvent>(evt);
        return this->sendKeyEvent(keyEvent);
    }
    else if (eventType == kEventTypeText) {
        std::shared_ptr<TextEvent> textEvent = std::dynamic_pointer_cast<TextEvent>(evt);
        return this->sendTextEvent(textEvent);
    }
    else if (eventType == kEventTypeMouseScroll) {
        std::shared_ptr<MouseScrollEvent> mouseScrollEvent = std::dynamic_pointer_cast<MouseScrollEvent>(evt);
        return this->sendMouseScrollEvent(mouseScrollEvent);
    }
    else if (eventType == kEventTypeMouseButton) {
        std::shared_ptr<MouseButtonEvent> mouseButtonEvent = std::dynamic_pointer_cast<MouseButtonEvent>(evt);
        return this->sendMouseButtonEvent(mouseButtonEvent);
    }
    else if (eventType == kEventTypeMouseMove) {
        std::shared_ptr<MouseMoveEvent> mouseMoveEvent = std::dynamic_pointer_cast<MouseMoveEvent>(evt);
        return this->sendMouseMoveEvent(mouseMoveEvent);
    }
    return false;
}

bool WindowManager::sendMouseMoveEvent(std::shared_ptr<MouseMoveEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    Point mouseLocation = makePoint(evt->getX(), evt->getY());
    std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
    if (window != nullptr) {
        std::shared_ptr<App> app = window->getApp().lock();
        Point locationInWindow = window->getLocationInWindow(mouseLocation);
        app->sendMouseMoveEvent(window->getId(), AspMouseEventMove, locationInWindow.x, locationInWindow.y, mouseLocation.x, mouseLocation.y);
        return true;
    }
    
    return false;
}

bool WindowManager::sendMouseButtonEvent(std::shared_ptr<MouseButtonEvent> evt)
{
    std::shared_ptr<Compositor> compositor = Server::getSingleton()->getCompositor().lock();
    Point mouseLocation = makePoint(evt->getX(), evt->getY());
    std::shared_ptr<Window> window = compositor->findWindowInLocation(mouseLocation);
    if (window != nullptr) {
        compositor->bringWindowToFront(window);
        std::shared_ptr<App> app = window->getApp().lock();
        Point locationInWindow = window->getLocationInWindow(mouseLocation);
        app->sendMouseButtonEvent(window->getId(), evt->getButtonEventType(), evt->getButton(), locationInWindow.x, locationInWindow.y, mouseLocation.x, mouseLocation.y);
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
        app->sendMouseWheelEvent(window->getId(), mouseLocation.x, mouseLocation.y, evt->getScrollX(), evt->getScrollY(), evt->getFlipped());
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
            app->sendTextEvent(topMostWindow->getId(), evt->getText());
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
            app->sendKeyEvent(topMostWindow->getId(), evt->getKeycode());
        }
    }
    return false;
}


