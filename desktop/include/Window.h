//
//  Surface.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_Surface_h
#define appserver_Surface_h

#include <memory>

#include <Geometry.h>
#include <Asl/Protocol.h>

namespace appserver
{
    class App;
    class Compositor;
    
    class Window
    {
    public:
        Window(std::weak_ptr<App> app, TWindowId id, const Rect& frame, int rasterType, bool visible = true);
        virtual ~Window();
        TWindowId getId();
        static TWindowId newWindowId();
        Rect getFrame() const;
        void setFrame(const Rect& frame);
        void setSize(const Size& size);
        void setLocation(const Point& location);
        bool isVisible() const;
        void setVisible(bool visible);
        TWindowZ getZ() const;
        void setZ(const TWindowZ z);
        
        virtual void performOperationsAndDraw() = 0;
        virtual bool operationsFinished() = 0;
        
        virtual void move(Point location) = 0;
        virtual void create(void *pixels, size_t bytes) = 0;
        virtual void resize(void *pixels, size_t bytes) = 0;
        virtual void updatePixels(void *pixels, size_t bytes, const Rect& dirtyRect) = 0;
        
        std::weak_ptr<App> getApp() const;
        unsigned long int getAppId() const;
        bool pointIsInsideTitleBar(Point p);
        bool pointIsInsideFrame(Point p);
        bool pointInInsideResizeHotspot(Point p);
        Point getLocationInWindow(Point locationInScreen);
        double getX() const;
        double getY() const;
        double getWidth() const;
        double getHeight() const;
        int getRasterType() const;
        void setRasterType(int rasterType);
    protected:
        static int compareZAsc(const void* a, const void* b);
        static int compareZDsc(const void* a, const void* b);
    public:
        constexpr static const double kMaxWidth = 10000.0f;
        constexpr static const double kMaxHeight = 10000.0f;
        constexpr static const unsigned long int kMaxDataSize = 52428800; // 50 Mb
    private:
        std::weak_ptr<App> _app;
        TWindowId _id;
        Rect _frame;
        bool _visible;
        TWindowZ _z;
        static TWindowId _idCounter;
        static TWindowId _zCounter;
        int _rasterType;
    };
}

#endif
