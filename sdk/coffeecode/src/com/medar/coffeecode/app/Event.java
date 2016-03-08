package com.medar.coffeecode.app;

public class Event {

    private EventType eventType;
    private long windowId;

    public Event() {
    }

    public Event(EventType eventType, long windowId) {
        this.eventType = eventType;
        this.windowId = windowId;
    }

    public EventType getEventType() {
        return eventType;
    }

    public void setEventType(EventType eventType) {
        this.eventType = eventType;
    }

    public long getWindowId() {
        return windowId;
    }

    public void setWindowId(long windowId) {
        this.windowId = windowId;
    }

    public enum EventType {
        INPUT,
        WINDOW_LOCATION_CHANGED
    }
}
