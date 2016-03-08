//
//  GLSurface.cpp
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <Server.h>
#include <SDLWindow.h>
#include <Geometry.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif

using namespace appserver;

SDLWindow::SDLWindow(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible)
    : Window(app, id, frame, rasterType, visible)
{
}

void SDLWindow::move(Point location)
{
}

void SDLWindow::create(void *pixels, size_t bytes)
{
    Uint32 pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    
    /*if (getRasterType() == AspWindowRasterBGRA) {
        pixelFormat = SDL_PIXELFORMAT_BGRA8888;
    }
    else if (getRasterType() == AspWindowRasterABGR) {
        pixelFormat = SDL_PIXELFORMAT_ABGR8888;
    }*/
    
    Rect frame = getFrame();
    _texture = SDL_CreateTexture(_renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, frame.size.width, frame.size.height);
    SDL_UpdateTexture(_texture, NULL, pixels, 4*frame.size.width);
}

void SDLWindow::resize(void *pixels, size_t bytes)
{
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    this->create(pixels, bytes);
}

void SDLWindow::updatePixels(void *pixels, size_t bytes, const Rect& dirtyRect)
{
    Rect frame = getFrame();
    SDL_UpdateTexture(_texture, NULL, pixels, 4*frame.size.width);
}


void SDLWindow::performOperationsAndDraw()
{
    Rect frame = getFrame();
    SDL_Rect dst;
    dst.x = frame.location.x;
    dst.y = frame.location.y;
    dst.w = frame.size.width;
    dst.h = frame.size.height;
    SDL_RenderCopy(_renderer, _texture, NULL, &dst);
}

bool SDLWindow::operationsFinished()
{
    return true;
}

void SDLWindow::setRenderer(SDL_Renderer *renderer)
{
    _renderer = renderer;
}

SDLWindow::~SDLWindow()
{
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
    }
}

