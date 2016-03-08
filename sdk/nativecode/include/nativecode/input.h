#ifndef _MANGO_INPUT_H
#define _MANGO_INPUT_H

namespace app
{
    typedef enum {
        kMouseEventTypeUndefined,
        kMouseEventTypePress,
        kMouseEventTypeRelease,
        kMouseEventTypeMove
    } MouseEventType;
    
    typedef enum {
        kMouseButtonUndefined,
        kMouseButtonRight,
        kMouseButtonLeft,
        kMouseButtonMiddle
    } MouseButton;
    
    typedef struct {
        unsigned int winId;
        MouseEventType type;
        MouseButton button;
        double x;
        double y;
        double absX;
        double absY;
        unsigned long viewId;
        int viewTag;
    } MouseEvent;
    
    typedef enum {
        kKeyEventUndefined,
        kKeyEventPress,
        kKeyEventRelease
    } KeyEventType;
    
    typedef struct {
        unsigned int winId;
        KeyEventType type;
        int charCode;
        int keyCode;
        unsigned long viewId;
        int viewTag;
    } KeyEvent;
    
    typedef enum {
        kInputEventTypeUndefined,
        kInputEventTypeMouse,
        kInputEventTypeKey
    } InputEventType;
    
    typedef struct {
        unsigned int winId;
        InputEventType type;
        MouseEvent *mouseEvent;
        KeyEvent *keyEvent;
    } InputEvent;
    
    class MouseEventDelegate
    {
    public:
        virtual void mouseEventOccured(MouseEvent *e) = 0;
    };
    
    class KeyEventDelegate
    {
    public:
        virtual void keyEventOccured(KeyEvent *e) = 0;
    };
}

#endif
