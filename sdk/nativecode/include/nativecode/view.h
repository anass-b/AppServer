#ifndef _MANGO_VIEW_H
#define _MANGO_VIEW_H

#include <iostream>
#include <vector>
#include <nativecode/geometry.h>
#include <nativecode/input.h>
#include <nativecode/graphics.h>

namespace app
{
    class Window;
    
    class View
    {
    public:
        View(Rect frame);
        void addSubView(View *child);
        void removeChildView(View *child);
        Rect getFrame();
        Rect getAbsoluteFrame();
        void render(Surface *parentSurf);
        std::vector<View*> getChildren();
        bool dispatchMouseEvent(MouseEvent *mouseEvent);
        void dispatchKeyEvent(KeyEvent *keyEvent);
        void addMouseEventDelegate(MouseEventDelegate *mouseDelegate);
        void addKeyEventDelegate(KeyEventDelegate *keyDelegate);
        void setParent(View *parent);
        void setWindow(Window *win);
        Window* getWindow();
        void setTag(int tag);
        int getTag();
        Surface* getSurface();
        void move(Point p);
        bool needsRedraw();
        void setNeedsRedraw();
        
    protected:
        virtual void draw() = 0;
        
    private:
        bool pointInsideVisualFrame(Point p);
        
    private:
        unsigned long _id = -1;
        Point _location;
        Surface *_surface = nullptr;
        static unsigned long _viewIdCounter;
        Window *_window = nullptr;
        int _tag = -1;
        bool _needsRedraw = true;
        
    public:
        View *_parent = nullptr;
        std::vector<View*> _subviews;
        std::vector<MouseEventDelegate*> _mouseEventDelegates;
        std::vector<KeyEventDelegate*> _keyEventDelegates;
    };
}

#endif
