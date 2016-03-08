package com.medar.coffeecode.app;

import com.medar.coffeecode.graphics.PointF;
import com.medar.coffeecode.graphics.RectF;
import com.medar.coffeecode.graphics.SizeF;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.concurrent.CopyOnWriteArrayList;

public class View {

    private static long idCounter = 0;
    private PointF location;
    private SizeF size;
    private long id;
    private BufferedImage bufferedImage;
    private View parent;
    private Window window;
    private long tag;
    private volatile boolean needsRedraw = true;
    private boolean drawOnly = false;
    private CopyOnWriteArrayList<View> subviews = new CopyOnWriteArrayList<View>();
    private ArrayList<MouseEventListener> mouseEventListeners = new ArrayList<MouseEventListener>();
    private ArrayList<KeyEventListener> keyEventListeners = new ArrayList<KeyEventListener>();
    private ArrayList<DrawEventListener> drawEventListeners = new ArrayList<DrawEventListener>();

    public View(RectF frame) {
        this(frame.location.x, frame.location.y, frame.size.width, frame.size.height);
    }

    public View(double x, double y, double width, double height) {
        ++idCounter;

        id = idCounter;

        location = new PointF(x, y);
        size = new SizeF(width, height);

        createBufferedImage(width, height);
    }

    void render(BufferedImage parentBufferedImage)
    {
        if (needsRedraw || drawOnly) {
            for (DrawEventListener l : drawEventListeners) {
                l.draw(bufferedImage.getGraphics());
            }
            needsRedraw = false;
            drawOnly = false;
        }

        for (View v : subviews) {
            v.render(bufferedImage);
        }

        if (parentBufferedImage != null) {
            parentBufferedImage.getGraphics().drawImage(bufferedImage, (int)location.x, (int)location.y, null);
        }
    }

    public void setDrawOnly(boolean drawOnly) {
        this.drawOnly = drawOnly;
    }

    public void setLocation(double x, double y) {
        location.x = x;
        location.y = y;
        markAsNeedsRedraw();
    }

    public void addSubView(View v) {
        v.parent = this;
        v.window = window;
        subviews.add(v);
        markAsNeedsRedraw();
    }

    void addSubViewWithoutRedraw(View v) {
        v.parent = this;
        v.window = window;
        subviews.add(v);
    }

    public boolean removedSubView(View v) {
        for (View view : subviews) {
            if (view == v) {
                subviews.remove(v);
                markAsNeedsRedraw();
                return true;
            }
        }
        return false;
    }

    private void createBufferedImage(double width, double height) {
        bufferedImage = new BufferedImage((int)width, (int)height, BufferedImage.TYPE_4BYTE_ABGR);
    }

    boolean needsRedraw() {
        if (needsRedraw) {
            return true;
        }

        for (View subview : subviews) {
            if (subview.needsRedraw()) {
                return true;
            }
        }

        return false;
    }

    void setNeedsRedraw(boolean needsRedraw) {
        this.needsRedraw = needsRedraw;
    }

    boolean dispatchMouseEvent(MouseEvent e) {
        for (int i = subviews.size()-1; i > 0; i--) {
            View subview = subviews.get(i);
            if (subview.dispatchMouseEvent(e)) {
                return true;
            }
        }

        if (pointInsideVisualFrame(e.getLocation())) {
            e.setViewId(id);
            e.setViewTag(tag);

            for (MouseEventListener l : mouseEventListeners) {
                l.mouseEventOccurred(e);
            }

            return true;
        }

        return false;
    }

    void dispatchKeyEvent(KeyEvent e) {
        e.setViewId(id);
        e.setViewTag(tag);

        for (KeyEventListener l : keyEventListeners) {
            l.keyEventOccurred(e);
        }
    }

    public void addMouseEventListener(MouseEventListener l) {
        mouseEventListeners.add(l);
    }

    public void addKeyEventListener(KeyEventListener l) {
        keyEventListeners.add(l);
    }

    public void addDrawEventListener(DrawEventListener l) {
        drawEventListeners.add(l);
    }

    boolean pointInsideVisualFrame(PointF p) {
        return pointInsideVisualFrame(p.x, p.y);
    }

    boolean pointInsideVisualFrame(double x, double y) {
        RectF frame = getFrameInWindow();
        if (x >= frame.location.x &&
                y >= frame.location.y &&
                x <= frame.location.x + frame.size.width &&
                y <= frame.location.y + frame.size.height) {
            if (parent != null) {
                RectF parentFrame = parent.getFrameInWindow();
                return x >= parentFrame.location.x &&
                        y >= parentFrame.location.y &&
                        x <= parentFrame.location.x + parentFrame.size.width &&
                        y <= parentFrame.location.y + parentFrame.size.height;
            } else {
                return true;
            }
        }
        return false;
    }

    RectF getFrameInWindow() {
        RectF frame = new RectF();
        if (parent != null) {
            RectF parentFrame = parent.getFrameInWindow();
            frame.location.x = parentFrame.location.x + this.location.x;
            frame.location.y = parentFrame.location.y + this.location.y;
        } else {
            frame.location.x = 0;
            frame.location.y = 0;
        }
        frame.size.width = this.size.width;
        frame.size.height = this.size.height;

        return new RectF(frame.location.x, frame.location.y, frame.size.width, frame.size.height);
    }

    public RectF getFrame() {
        return new RectF(location.x, location.y, size.width, size.height);
    }

    public double getX() {
        return location.x;
    }

    public double getY() {
        return location.y;
    }

    public double getWidth() {
        return size.width;
    }

    public double getHeight() {
        return size.height;
    }

    public void markAsNeedsRedraw() {
        this.needsRedraw = true;
    }

    void setWindow(Window window) {
        this.window = window;
    }

    public Window getWindow() {
        return window;
    }

    void setParent(View v) {
        parent = v;
        window = v.window;
    }

    public View getParent() {
        return parent;
    }

    public long getTag() {
        return tag;
    }

    public void setTag(long tag) {
        this.tag = tag;
    }

    void setSubviews(CopyOnWriteArrayList<View> subviews) {
        this.subviews = subviews;
    }

    CopyOnWriteArrayList<View> getSubviews() {
        return subviews;
    }

    public ArrayList<MouseEventListener> getMouseEventListeners() {
        return mouseEventListeners;
    }

    public void setMouseEventListeners(ArrayList<MouseEventListener> mouseEventListeners) {
        this.mouseEventListeners = mouseEventListeners;
    }

    public ArrayList<KeyEventListener> getKeyEventListeners() {
        return keyEventListeners;
    }

    public void setKeyEventListeners(ArrayList<KeyEventListener> keyEventListeners) {
        this.keyEventListeners = keyEventListeners;
    }

    public ArrayList<DrawEventListener> getDrawEventListeners() {
        return drawEventListeners;
    }

    public void setDrawEventListeners(ArrayList<DrawEventListener> drawEventListeners) {
        this.drawEventListeners = drawEventListeners;
    }

    BufferedImage getBufferedImage() {
        return bufferedImage;
    }

    public interface MouseEventListener {
        void mouseEventOccurred(MouseEvent e);
    }

    public interface KeyEventListener {
        void keyEventOccurred(KeyEvent e);
    }

    public interface DrawEventListener {
        void draw(Graphics g);
    }
}
