//
//  Compositor.cpp
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <compositor.h>
#include <iostream>
#include <algorithm>

using namespace appserver;
 
Compositor::Compositor()
{
}

void Compositor::addWindow(std::shared_ptr<Window> w)
{
    _windows.push_back(w);
}

void Compositor::removeWindow(TWindowId id)
{
    try {
        _windows.erase(findWindowPosition(id));
    } catch (std::exception e) {
        // TODO: Do something...
    }
}

void Compositor::removeWindows(unsigned long int appId)
{
    for (auto iter = _windows.begin(); iter != _windows.end();) {
        std::shared_ptr<Window> window = *iter;
        if (window->getAppId() == appId && window->getAppId() != -1) {
            _windows.erase(iter);
        }
        else {
            iter++;
        }
    }
}

bool Compositor::compareWindowsZDsc(std::shared_ptr<Window> winA, std::shared_ptr<Window> winB)
{
    return winA->getZ() < winB->getZ();
}

void Compositor::sortWindowsByZOrder()
{
    sort(_windows.begin(), _windows.end(), compareWindowsZDsc);
}

std::shared_ptr<Window> Compositor::findWindow(TWindowId id)
{
    for (auto iter = _windows.begin(); iter != _windows.end(); ++iter) {
        std::shared_ptr<Window> window = *iter;
        if (window->getId() == id) {
            return window;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Window>>::iterator Compositor::findWindowPosition(TWindowId id)
{
    for (auto iter = _windows.begin(); iter != _windows.end(); ++iter) {
        std::shared_ptr<Window> window = *iter;
        if (window->getId() == id) {
            return iter;
        }
    }
    throw std::invalid_argument("Window not found");
    
    return std::vector<std::shared_ptr<Window>>::iterator();
}

std::shared_ptr<Window> Compositor::findWindowInLocation(Point location)
{
    unsigned long size = _windows.size();
    for (int i = static_cast<int>(size) - 1; i >= 0 ; i--) {
        std::shared_ptr<Window> window = _windows.at(i);
        Rect windowFrame = window->getFrame();
        if (location.x >= windowFrame.location.x &&
            location.x <= windowFrame.location.x + windowFrame.size.width &&
            location.y >= windowFrame.location.y &&
            location.y <= windowFrame.location.y + windowFrame.size.height) {
            return window;
        }
    }
    return nullptr;
}

TWindowZ Compositor::computeMaxZ() const
{
    TWindowZ zMax = 0;
    unsigned long size = _windows.size();
    for (int i = static_cast<int>(size) - 1; i >= 0 ; i--) {
        std::shared_ptr<Window> window = _windows.at(i);
        TWindowZ z = window->getZ();
        if (z > zMax) {
            zMax = window->getZ();
        }
    }
    return zMax;
}

TWindowZ Compositor::getNewWindowZ() const
{
    return computeMaxZ() + 1;
}

void Compositor::bringWindowToFront(std::shared_ptr<Window> w)
{
    TWindowZ zMax = computeMaxZ();
    
    if (w->getZ() == zMax) {
        return;
    }
    
    w->setZ(zMax + 1);
    
    sortWindowsByZOrder();
}

std::shared_ptr<Window> Compositor::getTopMostWindow()
{
    if (_windows.size() > 0) {
        return _windows.back();
    }
    return nullptr;
}

void Compositor::setSize(Size size)
{
    _size = size;
}

Size Compositor::getSize() const
{
    return _size;
}

std::vector<std::shared_ptr<Window>>& Compositor::getWindows() const
{
    return _windows;
}

Compositor::~Compositor()
{
}

