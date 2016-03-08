#include <nativecode/view.h>
#include <nativecode/window.h>

namespace app
{
    unsigned long View::_viewIdCounter = 0;
    
    View::View(Rect frame)
    {
        ++_viewIdCounter;
        _id = _viewIdCounter;
        _location = frame.location;
        _surface = new Surface(frame.size);
    }
    
    void View::move(Point p)
    {
        _location = p;
        setNeedsRedraw();
    }
    
    void View::addSubView(View *child)
    {
        child->setParent(this);
        _subviews.push_back(child);
        setNeedsRedraw();
    }
    
    void View::removeChildView(View *child)
    {
        for (int i = 0; i < _subviews.size(); i++) {
            View *v = _subviews.at(i);
            if (v == child) {
                _subviews.erase(_subviews.begin() + i);
                return;
            }
        }
    }
    
    void View::render(Surface *parentSurf)
    {
        if (_needsRedraw) {
            draw();
            _needsRedraw = false;
        }
        
        for (int i = 0; i < _subviews.size(); i++) {
            View *v = _subviews.at(i);
            v->render(_surface);
        }
        
        if (parentSurf) {
            Size size = _surface->getSize();
            parentSurf->blit(_surface, makeRect(0.0, 0.0, size.width, size.height), _location);
        }
    }
    
    void View::setNeedsRedraw()
    {
        _needsRedraw = true;
    }
    
    bool View::needsRedraw()
    {
        if (_needsRedraw) {
            return true;
        }
        
        for (int i = 0; i < _subviews.size(); i++) {
            View *v = _subviews.at(i);
            if (v->needsRedraw()) {
                return true;
            }
        }
        
        return false;
    }
    
    bool View::pointInsideVisualFrame(Point p)
    {
        Rect frame = getAbsoluteFrame();
        if (p.x >= frame.location.x &&
            p.y >= frame.location.y &&
            p.x <= frame.location.x + frame.size.width &&
            p.y <= frame.location.y + frame.size.height) {
            if (_parent) {
                Rect parentFrame = _parent->getAbsoluteFrame();
                if (p.x >= parentFrame.location.x &&
                    p.y >= parentFrame.location.y &&
                    p.x <= parentFrame.location.x + parentFrame.size.width &&
                    p.y <= parentFrame.location.y + parentFrame.size.height) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        return false;
    }
    
    void View::addMouseEventDelegate(MouseEventDelegate *mouseDelegate)
    {
        if (mouseDelegate) {
            _mouseEventDelegates.push_back(mouseDelegate);
        }
    }
    
    void View::addKeyEventDelegate(KeyEventDelegate *keyDelegate)
    {
        if (keyDelegate) {
            _keyEventDelegates.push_back(keyDelegate);
        }
    }
    
    bool View::dispatchMouseEvent(MouseEvent *e)
    {
        for (unsigned long i = _subviews.size(); i > 0; i--) {
            View *v = _subviews.at(i-1);
            if (v->dispatchMouseEvent(e)) {
                return true;
            }
        }
        
        if (pointInsideVisualFrame(makePoint(e->x, e->y))) {
            e->viewId = _id;
            e->viewTag = _tag;
            for (int i = 0; i < _mouseEventDelegates.size(); i++) {
                MouseEventDelegate *delegate = _mouseEventDelegates.at(i);
                if (delegate) {
                    delegate->mouseEventOccured(e);
                }
            }
            return true;
        }
        
        return false;
    }
    
    void View::dispatchKeyEvent(KeyEvent *e)
    {
        e->viewId = _id;
        e->viewTag = _tag;
        
        for (int i = 0; i < _keyEventDelegates.size(); i++) {
            KeyEventDelegate *delegate = _keyEventDelegates.at(i);
            if (delegate) {
                delegate->keyEventOccured(e);
            }
        }
    }
    
    void View::setParent(View *parent)
    {
        if (_parent) return;
        _parent = parent;
    }
    
    void View::setWindow(Window *win)
    {
        if (_parent) {
            return;
        }
        _window = win;
    }
    
    Window* View::getWindow()
    {
        if (_parent) {
            return _parent->getWindow();
        }
        return _window;
    }
    
    Rect View::getAbsoluteFrame()
    {
        Size size = _surface->getSize();
        Rect frame;
        if (_parent) {
            Rect parentFrame = _parent->getAbsoluteFrame();
            frame.location.x = parentFrame.location.x + _location.x;
            frame.location.y = parentFrame.location.y + _location.y;
        } else {
            frame.location.x = 0;
            frame.location.y = 0;
        }
        frame.size.width = size.width;
        frame.size.height = size.height;
        
        return makeRect(frame.location.x, frame.location.y, frame.size.width, frame.size.height);
    }
    
    Rect View::getFrame()
    {
        Size size = _surface->getSize();
        return makeRect(_location.x, _location.y, size.width, size.height);
    }
    
    Surface* View::getSurface()
    {
        return _surface;
    }
    
    std::vector<View*> View::getChildren()
    {
        return _subviews;
    }
    
    void View::setTag(int tag)
    {
        _tag = tag;
    }
    
    int View::getTag()
    {
        return _tag;
    }
}

