package com.medar.coffeecode.app;

public class InputEvent extends Event {

    private InputEventType inputEventType;

    private long viewId;
    private long viewTag;

    public InputEvent(InputEventType inputEventType, long windowId, long viewId, long viewTag) {
        super(EventType.INPUT, windowId);
        this.inputEventType = inputEventType;
        this.viewId = viewId;
        this.viewTag = viewTag;
    }

    public InputEventType getInputEventType() {
        return inputEventType;
    }

    public void setInputEventType(InputEventType inputEventType) {
        this.inputEventType = inputEventType;
    }

    public long getViewId() {
        return viewId;
    }

    public void setViewId(long viewId) {
        this.viewId = viewId;
    }

    public long getViewTag() {
        return viewTag;
    }

    public void setViewTag(long viewTag) {
        this.viewTag = viewTag;
    }

    public enum InputEventType {
        KEY,
        MOUSE
    }
}
