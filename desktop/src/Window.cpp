//
//  Surface.cpp
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <Window.h>
#include <App.h>
#include <Server.h>

using namespace appserver;

TWindowId Window::_idCounter = 100000;
TWindowId Window::_zCounter = 0;

Window::Window(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible)
    : _app(app), _id(id), _frame(frame), _visible(visible), _z(Server::getSingleton()->getCompositor().lock()->getNewWindowZ()), _rasterType(rasterType)
{
}

bool Window::pointIsInsideTitleBar(Point p)
{
    if (p.x >= _frame.location.x &&
        p.y >= _frame.location.y &&
        p.x <= _frame.location.x + _frame.size.width &&
        p.y <= _frame.location.y + 40) {
        return true;
    }
    return false;
}

bool Window::pointIsInsideFrame(Point p)
{
    if (p.x >= _frame.location.x &&
        p.y >= _frame.location.y &&
        p.x <= _frame.location.x + _frame.size.width &&
        p.y <= _frame.location.y + _frame.size.height) {
        return true;
    }
    return false;
}

Point Window::getLocationInWindow(Point locationInScreen)
{
    return makePoint(locationInScreen.x - _frame.location.x, locationInScreen.y - _frame.location.y);
}

bool Window::pointInInsideResizeHotspot(Point p)
{
    if (p.x >= getWidth() - 40 &&
        p.y >= getHeight() - 40 &&
        p.x <= getWidth() &&
        p.y <= getHeight()) {
        return true;
    }
    return false;
}

TWindowId Window::newWindowId()
{
    return ++_idCounter;
}

TWindowId Window::getId()
{
    return _id;
}

Rect Window::getFrame() const
{
    return _frame;
}

void Window::setFrame(const Rect& frame)
{
    _frame = frame;
}

void Window::setSize(const Size& size)
{
    _frame.size = size;
}

void Window::setLocation(const Point& location)
{
    _frame.location = location;
}

bool Window::isVisible() const
{
    return _visible;
}

double Window::getX() const
{
    return _frame.location.x;
}

double Window::getY() const
{
    return _frame.location.y;
}

double Window::getWidth() const
{
    return _frame.size.width;
}

double Window::getHeight() const
{
    return _frame.size.height;
}

void Window::setVisible(bool visible)
{
    _visible = visible;
}

TWindowZ Window::getZ() const
{
    return _z;
}

void Window::setZ(const TWindowZ z)
{
    _z = z;
}

int Window::getRasterType() const
{
    return _rasterType;
}

void Window::setRasterType(int rasterType)
{
    _rasterType = rasterType;
}

std::weak_ptr<App> Window::getApp() const
{
    return _app;
}

unsigned long int Window::getAppId() const
{
    std::shared_ptr<App> app = _app.lock();
    if (app != nullptr) {
        return app->getId();
    }
    return -1;
}

Window::~Window()
{
}

