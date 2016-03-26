//
//  GLSurface.cpp
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <server.h>
#include <sdlwindow.h>
#include <geometry.h>

using namespace appserver;

SDLWindow::SDLWindow(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible):
    Window(app, id, frame, rasterType, visible),
    _glTexOperation(kSDLTexOpNone),
    _cachedFrame(frame),
    _pixels(nullptr),
    _glOpBlocked(false),
    _dataOpBlocked(false),
    _dirtyRect(makeRect(0.0, 0.0, 0.0, 0.0))    
{
}

void SDLWindow::move(Point location)
{
    _cachedFrame.location = location;
}

void SDLWindow::create(void *pixels, size_t bytes)
{
    while (_dataOpBlocked);
    _glOpBlocked = true;
    
    if (_pixels != nullptr) {
        free(_pixels);
        _pixels = nullptr;
    }
    _pixels = pixels;
    _glTexOperation = kSDLTexOpCreate;
    
    _glOpBlocked = false;
}

void SDLWindow::resize(void *pixels, size_t bytes)
{
    while (_dataOpBlocked);
    _glOpBlocked = true;
    
    if (_pixels != nullptr) {
        free(_pixels);
        _pixels = nullptr;
    }
    _cachedFrame = getFrame();
    _pixels = pixels;
    _glTexOperation = kSDLTexOpResize;
    
    _glOpBlocked = false;
}

void SDLWindow::updatePixels(void *pixels, size_t bytes, const Rect& dirtyRect)
{
    while (_dataOpBlocked);
    _glOpBlocked = true;
    
    if (_pixels != nullptr) {
        free(_pixels);
        _pixels = nullptr;
    }
    _pixels = pixels;
    _dirtyRect = dirtyRect;
    _glTexOperation = kSDLTexOpUpdatePixels;
    
    _glOpBlocked = false;
}


void SDLWindow::performOperationsAndDraw()
{
    
    while (_glOpBlocked);
    _dataOpBlocked = true;
    
    switch (_glTexOperation) {
        case kSDLTexOpCreate:
            createTexture();
            break;
            
        case kSDLTexOpUpdatePixels:
            updateTexturePixels();
            break;
            
        case kSDLTexOpResize:
            resizeTexture();
            break;
            
        default:
            break;
    }
    _glTexOperation = kSDLTexOpNone;
    _dataOpBlocked = false;
    
    draw();
}

bool SDLWindow::operationsFinished()
{
    return true;
}

void SDLWindow::setRenderer(SDL_Renderer *renderer)
{
    _renderer = renderer;
}

void SDLWindow::createTexture()
{
    Uint32 pixelFormat = SDL_PIXELFORMAT_ARGB8888;
    
    if (getRasterType() == AspWindowRasterRGBA) {
        pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    }
    else if (getRasterType() == AspWindowRasterARGB) {
        pixelFormat = SDL_PIXELFORMAT_ARGB8888;
    }
    
    Rect frame = getCachedFrame();
    _texture = SDL_CreateTexture(_renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, frame.size.width, frame.size.height);
    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(_texture, NULL, _pixels, 4*frame.size.width);
}

void SDLWindow::resizeTexture()
{
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    this->createTexture();
}

void SDLWindow::updateTexturePixels()
{
    Rect frame = getFrame();
    SDL_UpdateTexture(_texture, NULL, _pixels, 4*frame.size.width);
}

void SDLWindow::draw()
{
    Rect frame = getCachedFrame();
    SDL_Rect dst;
    dst.x = frame.location.x;
    dst.y = frame.location.y;
    dst.w = frame.size.width;
    dst.h = frame.size.height;
    SDL_RenderCopy(_renderer, _texture, NULL, &dst);
}

Rect SDLWindow::getCachedFrame() const
{
    return _cachedFrame;
}

SDLWindow::~SDLWindow()
{
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
    }
}

