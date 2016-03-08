package com.medar.coffeecode.graphics;

public class RectF {
    public PointF location;
    public SizeF size;

    public RectF() {
        location = new PointF(0.0, 0.0);
        size = new SizeF(0.0, 0.0);
    }

    public RectF(double x, double y, double width, double height) {
        location = new PointF(x, y);
        size = new SizeF(width, height);
    }
}
