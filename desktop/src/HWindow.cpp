//
//  GLSurface.cpp
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <Server.h>
#include <HWindow.h>
#include <Geometry.h>
#include <OpenGL/glu.h>

using namespace appserver;

HWindow::HWindow(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible)
    : Window(app, id, frame, rasterType, visible), _texId(0), _glOpBlocked(false),
        _glTexOperation(kTexOpNone), _pixels(nullptr), _dirtyRect(makeRect(0.0, 0.0, 0.0, 0.0)), _dataOpBlocked(false), _cachedFrame(frame)
{
}

void HWindow::move(Point location)
{
    _cachedFrame.location = location;
}

void HWindow::create(void *pixels, size_t bytes)
{
    while (_dataOpBlocked);
    _glOpBlocked = true;
    
    if (_pixels != nullptr) {
        free(_pixels);
        _pixels = nullptr;
    }
    _pixels = pixels;
    _glTexOperation = kTexOpCreate;
    
    _glOpBlocked = false;
}

void HWindow::resize(void *pixels, size_t bytes)
{
    while (_dataOpBlocked);
    _glOpBlocked = true;
    
    if (_pixels != nullptr) {
        free(_pixels);
        _pixels = nullptr;
    }
    _cachedFrame = getFrame();
    _pixels = pixels;
    _glTexOperation = kTexOpResize;
    
    _glOpBlocked = false;
}

void HWindow::updatePixels(void *pixels, size_t bytes, const Rect& dirtyRect)
{
    while (_dataOpBlocked);
    _glOpBlocked = true;
    
    if (_pixels != nullptr) {
        free(_pixels);
        _pixels = nullptr;
    }
    _pixels = pixels;
    _dirtyRect = dirtyRect;
    _glTexOperation = kTexOpUpdatePixels;
    
    _glOpBlocked = false;
}


void HWindow::performOperationsAndDraw()
{
    while (_glOpBlocked);
    _dataOpBlocked = true;
    
    switch (_glTexOperation) {
        case kTexOpCreate:
            glCreateTexture();
            break;
            
        case kTexOpUpdatePixels:
            glUpdateTexturePixels();
            break;
            
        case kTexOpResize:
            glResizeTexture();
            break;
            
        default:
            break;
    }
    _glTexOperation = kTexOpNone;
    _dataOpBlocked = false;
    
    glDraw();
}

void HWindow::glCreateTexture()
{
    Rect frame = getCachedFrame();
    
    glGenTextures(1, &_texId);
    glBindTexture(GL_TEXTURE_2D, _texId);
    
    if (getRasterType() == AspWindowRasterBGRA) {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, frame.size.width, frame.size.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, _pixels);
    }
    else if (getRasterType() == AspWindowRasterABGR) {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, frame.size.width, frame.size.height, 0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, _pixels);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void HWindow::glResizeTexture()
{
    Rect frame = getCachedFrame();
    
    glBindTexture(GL_TEXTURE_2D, _texId);
    if (getRasterType() == AspWindowRasterBGRA) {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, frame.size.width, frame.size.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, _pixels);
    }
    else if (getRasterType() == AspWindowRasterABGR) {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, frame.size.width, frame.size.height, 0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, _pixels);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void HWindow::glUpdateTexturePixels()
{
    Rect dirtyRect = _dirtyRect;
    
    glBindTexture(GL_TEXTURE_2D, _texId);
    
    if (getRasterType() == AspWindowRasterBGRA) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, dirtyRect.location.x, dirtyRect.location.y, dirtyRect.size.width, dirtyRect.size.height, GL_BGRA, GL_UNSIGNED_BYTE, _pixels);
    }
    else if (getRasterType() == AspWindowRasterABGR) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, dirtyRect.location.x, dirtyRect.location.y, dirtyRect.size.width, dirtyRect.size.height, GL_ABGR_EXT, GL_UNSIGNED_BYTE, _pixels);
    }
}

void HWindow::glDeleteTexture()
{
    //GLuint uint = (GLuint)_texId;
    //glDeleteTextures(1, &uint);
}

void HWindow::glDraw()
{
    Rect frame = getCachedFrame();
    
    glBindTexture(GL_TEXTURE_2D, _texId);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    
    // Top left
    glTexCoord2i(0, 0);
    glVertex3f(frame.location.x, frame.location.y, 0);
    
    // Top right
    glTexCoord2i(1, 0);
    glVertex3f(frame.location.x + frame.size.width, frame.location.y, 0);
    
    // Bottom right
    glTexCoord2i(1, 1);
    glVertex3f(frame.location.x + frame.size.width, frame.location.y + frame.size.height, 0);
    
    // Bottom left
    glTexCoord2i(0, 1);
    glVertex3f(frame.location.x, frame.location.y + frame.size.height, 0);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

bool HWindow::operationsFinished()
{
    if (!_dataOpBlocked) {
        return true;
    }
    return false;
}

Rect HWindow::getCachedFrame() const
{
    return _cachedFrame;
}

HWindow::~HWindow()
{
    glDeleteTexture();
}

