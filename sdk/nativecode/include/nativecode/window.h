#ifndef _MANGO_WINDOW_H
#define _MANGO_WINDOW_H

#include <nativecode/input.h>
#include <nativecode/view.h>
#include <nativecode/geometry.h>

namespace app
{
    typedef enum
    {
        kWindowVisibilityHidden = 0,
        kWindowVisibilityVisible = 1
    } WindowVisibility;
    
    class RootView : public View
    {
    public:
        RootView(Rect frame);
    protected:
        virtual void draw();
    };
    
    class ContentView : public View
    {
    public:
        ContentView(Rect frame);
    protected:
        virtual void draw();
    };
    
    class Window
    {
    public:
        friend class Application;
        Window(Rect frame, WindowVisibility visibility);
        int dragWindowOnEvent(InputEvent *evt);
        bool bringWindowToFront();
        bool moveWindow(Point newLocation);
        void resizeWindow(Size newSize);
        void addView(View *v);
        void flushContent();
        int getId();
        void dispatchInputEvent(InputEvent *e);
        View *getFocusedView();
        void setFocusedView(View *v);
        View *getContentView();
        void addMouseEventDelegate(MouseEventDelegate *mouseDelegate);
        void addKeyEventDelegate(KeyEventDelegate *keyDelegate);
        Size getSize() const;
        Point getLocation() const;
        void setWindowManagementEnabled(bool enable);
    private:
        void flush();
        void composeViews(bool flush);
        void createContentView();
        void createRootView();
        bool windowVisibilityToBool(WindowVisibility visibility);
        Point* mouseLocationInContentView(Point absoluteLocation);
        View* getRootView() const;
    private:
        int _id = -1;
        Rect _frame;
        bool _mouseHeld = false;
        double _initialX = 0.0;
        double _initialY = 0.0;
        RootView *_rootView = nullptr;
        ContentView *_contentView = nullptr;
        View *_focusedView = nullptr;
        double _winMarginRight = 10.0;
        double _winMarginLeft = 10.0;
        double _winMarginTop = 40.0;
        double _winMarginBottom = 10.0;
        bool _serverWinCreated = false;
        std::vector<MouseEventDelegate*> *_mouseEventDelegates = nullptr;
        std::vector<KeyEventDelegate*> *_keyEventDelegates = nullptr;
        bool _windowManagementEnabled = true;
    };
}

#endif
