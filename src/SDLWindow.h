//
//  GLSurface.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <memory>
#include <GLCompositor.h>
#include <SDL.h>

namespace appserver
{
    enum SDLTexOperation {
        kSDLTexOpNone,
        kSDLTexOpCreate,
        kSDLTexOpUpdatePixels,
        kSDLTexOpResize
    };
    
    class SDLWindow : public Window
    {
    public:
        SDLWindow(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible = true);
        SDLWindow(TWindowId id);
        virtual ~SDLWindow();
        
        virtual void performOperationsAndDraw();
        virtual bool operationsFinished();
        
        virtual void move(Point location);
        virtual void create(void *pixels, size_t bytes);
        virtual void resize(void *pixels, size_t bytes);
        virtual void updatePixels(void *pixels, size_t bytes, const Rect& dirtyRect);
        
        void setRenderer(SDL_Renderer *renderer);
        
    private:
        void draw();
        void createTexture();
        void resizeTexture();
        void updateTexturePixels();
        
        Rect getCachedFrame() const;
        
    private:        
        SDLTexOperation _glTexOperation;
        Rect _cachedFrame;
        void* _pixels;
        bool _glOpBlocked;
        bool _dataOpBlocked;
        Rect _dirtyRect;
        SDL_Texture *_texture;
        SDL_Renderer *_renderer;
    };
}

#endif
