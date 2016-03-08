package com.medar.coffeecode.app;

class WindowLocationChangedEvent extends Event {

    private double newX;
    private double newY;

    WindowLocationChangedEvent(long windowId, double newX, double newY) {
        super(EventType.WINDOW_LOCATION_CHANGED, windowId);
        this.newX = newX;
        this.newY = newY;
    }

    public double getNewX() {
        return newX;
    }

    public void setNewX(double newX) {
        this.newX = newX;
    }

    public double getNewY() {
        return newY;
    }

    public void setNewY(double newY) {
        this.newY = newY;
    }
}
