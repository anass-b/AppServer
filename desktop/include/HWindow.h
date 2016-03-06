//
//  GLSurface.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_GLSurface_h
#define appserver_GLSurface_h

#include <memory>

#include <Window.h>
#include <OpenGL/OpenGL.h>
#include <HCompositor.h>

namespace appserver
{
    enum GLTexOperation {
        kTexOpNone,
        kTexOpCreate,
        kTexOpUpdatePixels,
        kTexOpResize
    };
    
    class HWindow : public Window
    {
    public:
        HWindow(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible = true);
        HWindow(TWindowId id);
        virtual ~HWindow();
        
        virtual void performOperationsAndDraw();
        virtual bool operationsFinished();
        
        virtual void move(Point location);
        virtual void create(void *pixels, size_t bytes);
        virtual void resize(void *pixels, size_t bytes);
        virtual void updatePixels(void *pixels, size_t bytes, const Rect& dirtyRect);
        
    private:
        void glDraw();
        void glCreateTexture();
        void glResizeTexture();
        void glUpdateTexturePixels();
        void glDeleteTexture();
        
        Rect getCachedFrame() const;
        
    private:
        Rect _cachedFrame;
        GLuint _texId;
        Rect _dirtyRect;
        void* _pixels;
        
        GLTexOperation _glTexOperation;
        bool _glOpBlocked;
        bool _dataOpBlocked;
    };
}

#endif
