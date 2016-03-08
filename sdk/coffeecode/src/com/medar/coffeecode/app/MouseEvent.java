package com.medar.coffeecode.app;

import com.medar.coffeecode.graphics.PointF;

public class MouseEvent extends InputEvent {

    private MouseEventType mouseEventType;
    private MouseButtonType mouseButtonType;
    private PointF location;
    private PointF locationInScreen;

    public MouseEvent(MouseEventType mouseEventType, MouseButtonType mouseButtonType, long windowId, long viewId, long viewTag , PointF location, PointF locationInScreen) {
        super(InputEventType.MOUSE, windowId, viewId, viewTag);
        this.mouseEventType = mouseEventType;
        this.mouseButtonType = mouseButtonType;
        this.location = location;
        this.locationInScreen = locationInScreen;
    }

    public MouseEventType getMouseEventType() {
        return mouseEventType;
    }

    public void setMouseEventType(MouseEventType mouseEventType) {
        this.mouseEventType = mouseEventType;
    }

    public MouseButtonType getMouseButtonType() {
        return mouseButtonType;
    }

    public void setMouseButtonType(MouseButtonType mouseButtonType) {
        this.mouseButtonType = mouseButtonType;
    }

    public PointF getLocation() {
        return location;
    }

    public void setLocation(PointF location) {
        this.location = location;
    }

    public PointF getLocationInScreen() {
        return locationInScreen;
    }

    public void setLocationInScreen(PointF locationInScreen) {
        this.locationInScreen = locationInScreen;
    }

    public enum MouseButtonType {
        UNDEFINED,
        RIGHT,
        LEFT,
        MIDDLE,
    }

    public enum MouseEventType {
        UNDEFINED,
        PRESS,
        RELEASE,
        MOVE,
        DRAG
    }
}
