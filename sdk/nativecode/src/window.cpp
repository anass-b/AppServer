#include <nativecode/window.h>
#include <private/server.h>
#include <nativecode/application.h>

using namespace asl;

namespace app
{
    /*
     * RootView
     */
    
    RootView::RootView(Rect frame) : View(frame)
    {
    }
    
    void RootView::draw()
    {
        Surface *surf = getSurface();
        surf->fill(makeColor(128, 128, 128));
    }
    
    /*
     * ContentView
     */
    
    ContentView::ContentView(Rect frame) : View(frame)
    {
    }
    
    void ContentView::draw()
    {
        Surface *surf = getSurface();
        surf->fill(makeColor(255, 255, 255));
    }
    
    /*
     * Window
     */
    
    Window::Window(Rect frame, WindowVisibility visibility)
    {
        _frame = frame;
        
        _rootView = new RootView(makeRect(0.0, 0.0, frame.size.width, frame.size.height));
        _rootView->setWindow(this);
        
        _contentView = new ContentView(makeRect(_winMarginLeft, _winMarginTop,
                                                _frame.size.width - _winMarginLeft - _winMarginRight,
                                                _frame.size.height - _winMarginTop - _winMarginBottom));
        _contentView->setTag(1000);
        _rootView->addSubView((View*)_contentView);
        _rootView->render(nullptr);
        
        Surface *rootViewSurf = _rootView->getSurface();
        Size surfSize = rootViewSurf->getSize();
        
        unsigned char *data = rootViewSurf->getData();
        unsigned long dataSize = rootViewSurf->getDataSize();
        
        _id = Application::getSingleton()->getServer()->createWindow(data, dataSize, frame.location.x, frame.location.y,
                                                                             frame.size.width, frame.size.height, windowVisibilityToBool(visibility));
        
        _frame.size.width = (frame.size.width == -1) ? surfSize.width : frame.size.width;
        _frame.size.height = (frame.size.height == -1) ? surfSize.height : frame.size.height;
        _mouseHeld = 0;
        _initialX = 0;
        _initialY = 0;
        
        Application::getSingleton()->addWindow(this);
        
        _mouseEventDelegates = new std::vector<MouseEventDelegate*>();
        _keyEventDelegates = new std::vector<KeyEventDelegate*>();
        
        _serverWinCreated = true;
    }
    
    void Window::addView(View *v)
    {
        _contentView->addSubView(v);
        _contentView->setWindow(this);
    }
    
    bool Window::windowVisibilityToBool(WindowVisibility visibility)
    {
        if (visibility == true) {
            return kWindowVisibilityVisible;
        } else {
            return kWindowVisibilityHidden;
        }
    }
    
    View* Window::getContentView()
    {
        return _contentView;
    }
    
    void Window::setWindowManagementEnabled(bool enable)
    {
        _windowManagementEnabled = enable;
    }
    
    int Window::dragWindowOnEvent(InputEvent *evt)
    {
        if (!evt) return 1;
        if (!evt->mouseEvent) return 1;
        if (evt->winId != _id) return 1;
        
        if (evt->mouseEvent->type == kMouseEventTypePress) {
            _mouseHeld = 1;
            _initialX = evt->mouseEvent->x;
            _initialY = evt->mouseEvent->y;
            
            bringWindowToFront();
            
            return 0;
        }
        else if (evt->mouseEvent->type == kMouseEventTypeRelease) {
            _mouseHeld = false;
            _initialX = 0;
            _initialY = 0;
        }
        
        if (_mouseHeld) {
            double new_x = evt->mouseEvent->absX - _initialX;
            double new_y = evt->mouseEvent->absY - _initialY;
            moveWindow(makePoint(new_x, new_y));
        }
        
        return 0;
    }
    
    bool Window::bringWindowToFront()
    {
        int result = Application::getSingleton()->getServer()->bringWindowToFront(_id);
        if (result != 0) {
            return false;
        }
        return true;
    }
    
    void Window::addMouseEventDelegate(MouseEventDelegate *mouseDelegate)
    {
        if (mouseDelegate) {
            _mouseEventDelegates->push_back(mouseDelegate);
        }
    }
    
    void Window::addKeyEventDelegate(KeyEventDelegate *keyDelegate)
    {
        if (keyDelegate) {
            _keyEventDelegates->push_back(keyDelegate);
        }
    }
    
    bool Window::moveWindow(Point newLocation)
    {
        int result = Application::getSingleton()->getServer()->moveWindow(_id, newLocation.x, newLocation.y);
        if (result != 0) {
            return false;
        }
        
        return true;
    }
    
    View* Window::getFocusedView()
    {
        return _focusedView;
    }
    
    void Window::setFocusedView(View *v)
    {
        _focusedView = v;
    }
    
    void Window::dispatchInputEvent(InputEvent *e)
    {
        if (e == nullptr) return;
        if (e->type == kInputEventTypeKey) {
            for (int i = 0; i < _keyEventDelegates->size(); i++) {
                KeyEventDelegate *delegate = _keyEventDelegates->at(i);
                if (delegate) {
                    delegate->keyEventOccured(e->keyEvent);
                }
            }
            
            if (_focusedView) {
                _focusedView->dispatchKeyEvent(e->keyEvent);
            } else {
                _rootView->dispatchKeyEvent(e->keyEvent);
            }
        }
        else if (e->type == kInputEventTypeMouse) {
            for (int i = 0; i < _mouseEventDelegates->size(); i++) {
                MouseEventDelegate *delegate = _mouseEventDelegates->at(i);
                if (delegate) {
                    delegate->mouseEventOccured(e->mouseEvent);
                }
            }
            
            // Handle Drag & Drop and Quit App
            if (e->mouseEvent) {
                if (e->mouseEvent->button == kMouseButtonRight &&
                    e->mouseEvent->type == kMouseEventTypePress) {
                    Application::getSingleton()->quit();
                }
                
                /*if (_windowManagementEnabled) {
                    dragWindowOnEvent(e);
                }*/
                
                _rootView->dispatchMouseEvent(e->mouseEvent);
            }
        }
    }
    
    Size Window::getSize() const
    {
        return _frame.size;
    }
    
    Point Window::getLocation() const
    {
        return _frame.location;
    }
    
    View* Window::getRootView() const
    {
        return _rootView;
    }
    
    void Window::resizeWindow(Size newSize)
    {
        if (!_serverWinCreated) return;
        
        _frame.size = newSize;
        
        // Create new RootView and ContentView
        RootView *newRootView = new RootView(makeRect(0.0, 0.0, newSize.width, newSize.height));
        newRootView->setWindow(this);
        
        ContentView *newContentView = new ContentView(makeRect(_winMarginLeft, _winMarginTop,
                                                               _frame.size.width - _winMarginLeft - _winMarginRight,
                                                               _frame.size.height - _winMarginTop - _winMarginBottom));
        newContentView->setTag(1000);
        newRootView->addSubView((View*)newContentView);
        // Copy
        newContentView->_subviews = _contentView->_subviews;
        // Re-assign parent for children
        for (unsigned long i = newContentView->_subviews.size(); i > 0; i--) {
            View *v = newContentView->_subviews.at(i-1);
            v->_parent = newContentView;
        }
        newContentView->_mouseEventDelegates = _contentView->_mouseEventDelegates;
        newContentView->_keyEventDelegates = _contentView->_keyEventDelegates;
        
        delete _rootView;
        delete _contentView;
        
        // Re-assign
        _rootView = newRootView;
        _contentView = newContentView;
        
        _rootView->render(nullptr);
        
        Surface *surf = _rootView->getSurface();
        unsigned char *data = surf->getData();
        unsigned long dataSize = surf->getDataSize();
        
        Application::getSingleton()->getServer()->resizeWindow(_id, data, dataSize, _frame.size.width, _frame.size.height);
    }
    
    void Window::flush()
    {
        if (!_serverWinCreated) return;
        
        _rootView->render(nullptr);
        
        Surface *surf = _rootView->getSurface();
        unsigned char *data = surf->getData();
        unsigned long dataSize = surf->getDataSize();
        Application::getSingleton()->getServer()->updateWindowSurface(_id, data, dataSize, 0.0f, 0.0f, _frame.size.width, _frame.size.height);
    }
    
    int Window::getId()
    {
        return _id;
    }
}

