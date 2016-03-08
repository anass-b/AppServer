package com.medar.coffeecode.graphics;

public class Rect {
    public Point location;
    public Size size;

    public Rect() {
        location = new Point(0, 0);
        size = new Size(0, 0);
    }

    public Rect(int x, int y, int width, int height) {
        location = new Point(x, y);
        size = new Size(width, height);
    }
}
