package com.medar.coffeecode.app;

import com.medar.coffeecode.graphics.NinePatch;
import com.medar.coffeecode.graphics.PointF;
import com.medar.coffeecode.graphics.RectF;
import com.medar.coffeecode.graphics.SizeF;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.nio.ByteBuffer;
import java.util.ArrayList;


public class Window {

    private long id;
    private PointF location;
    private SizeF size;
    private RectF contentFrame;
    private RootView rootView;
    private ContentView contentView;
    private View focusedView;
    private boolean serverWindowCreated;
    private String title;
    private ArrayList<MouseEventListener> mouseEventListeners = new ArrayList<MouseEventListener>();
    private ArrayList<KeyEventListener> keyEventListeners = new ArrayList<KeyEventListener>();
    public final static long ROOT_VIEW_TAG = 0;
    public final static long CONTENT_VIEW_TAG = 1;
    private final static double MARGIN_RIGHT = 5.0;
    private final static double MARGIN_LEFT = 5.0;
    private final static double MARGIN_TOP = 40.0;
    private final static double MARGIN_BOTTOM = 7.0;
    private final static double DEFAULT_WIDTH = 240.0;
    private final static double DEFAULT_HEIGHT = 160.0;
    private final static double MIN_WIDTH = 50.0;
    private final static double MIN_HEIGHT = 50.0;
    private final static String DEFAULT_TITLE = "";

    public Window() {
        this(DEFAULT_TITLE, 0.0, 0.0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    public Window(String title, double x, double y, double width, double height) {
        this.title = title;

        location = new PointF(x, y);
        size = new SizeF(MARGIN_LEFT + width + MARGIN_RIGHT, MARGIN_TOP + height + MARGIN_BOTTOM);

        contentFrame = new RectF(MARGIN_LEFT, MARGIN_TOP, width, height);
        contentView = new ContentView(contentFrame);
        contentView.setTag(CONTENT_VIEW_TAG);

        rootView = new RootView(0.0, 0.0, size.width, size.height);
        rootView.setTag(ROOT_VIEW_TAG);
        rootView.setWindow(this);
        rootView.addSubView(contentView);

        CloseButton closeButton = new CloseButton(16, 12, 20, 20);
        rootView.addSubView(closeButton);
    }

    void flush() {
        if (!serverWindowCreated) return;
        rootView.render(null);

        BufferedImage rootViewBufferImage = rootView.getBufferedImage();

        byte[] pixels = ((DataBufferByte) rootViewBufferImage.getRaster().getDataBuffer()).getData();

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pixels.length);
        byteBuffer.put(pixels);
        byteBuffer.position(0);

        Pointer pointer = Native.getDirectBufferPointer(byteBuffer);

        Asl.INSTANCE.aslUpdateWindowSurface(id, pointer, pixels.length, 0.0, 0.0, size.width, size.height);
    }

    public void show() {
        if (!serverWindowCreated) {
            rootView.render(null);

            BufferedImage rootViewBufferImage = rootView.getBufferedImage();

            byte[] pixels = ((DataBufferByte) rootViewBufferImage.getRaster().getDataBuffer()).getData();

            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pixels.length);
            byteBuffer.put(pixels);
            byteBuffer.position(0);

            Pointer pointer = Native.getDirectBufferPointer(byteBuffer);

            id = Asl.INSTANCE.aslNewWindow(pointer, pixels.length, location.x, location.y, size.width, size.height, Asl.WINDOW_RASTER_TYPE_RGBA, true);
            App.getInstance().addWindow(this);

            serverWindowCreated = true;
        }
        else {
            Asl.INSTANCE.aslChangeWindowVisibility(id, true);
        }
    }

    public void resize(double width, double height) {

        long startTime = System.nanoTime();
        contentFrame.size.width = width;
        contentFrame.size.height = height;
        size.width = MARGIN_LEFT + width + MARGIN_RIGHT;
        size.height = MARGIN_TOP + height + MARGIN_BOTTOM;

        if (!serverWindowCreated) return;

        ContentView newContentView = new ContentView(contentFrame);
        newContentView.setNeedsRedraw(false);
        newContentView.setDrawOnly(true);
        newContentView.setTag(CONTENT_VIEW_TAG);

        // TODO: To be optimized
        // Copy subviews into the new contentView
        newContentView.setSubviews(contentView.getSubviews());
        for (View v : newContentView.getSubviews()) {
            v.setParent(newContentView);
        }
        newContentView.setDrawEventListeners(newContentView.getDrawEventListeners());
        newContentView.setMouseEventListeners(newContentView.getMouseEventListeners());
        newContentView.setKeyEventListeners(newContentView.getKeyEventListeners());

        RootView newRootView = new RootView(0.0, 0.0, size.width, size.height);
        newRootView.setNeedsRedraw(false);
        newRootView.setDrawOnly(true);
        newRootView.setTag(ROOT_VIEW_TAG);
        newRootView.setWindow(this);
        newRootView.addSubViewWithoutRedraw(newContentView);

        CloseButton closeButton = new CloseButton(16, 12, 20, 20);
        closeButton.setNeedsRedraw(false);
        closeButton.setDrawOnly(true);
        newRootView.addSubViewWithoutRedraw(closeButton);

        rootView = newRootView;
        contentView = newContentView;

        rootView.render(null);

        BufferedImage rootViewBufferImage = rootView.getBufferedImage();
        byte[] pixels = ((DataBufferByte) rootViewBufferImage.getRaster().getDataBuffer()).getData();

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pixels.length);
        byteBuffer.put(pixels);
        byteBuffer.position(0);

        Pointer pointer = Native.getDirectBufferPointer(byteBuffer);

        Asl.INSTANCE.aslResizeWindow(id, pointer, pixels.length, size.width, size.height);

        long endTime = System.nanoTime();
        double difference = (endTime - startTime)/1e6;
        //System.out.println("resizing ("+ difference +")");
    }

    public void hide() {
        Asl.INSTANCE.aslChangeWindowVisibility(id, false);
    }

    public void destroy() {
        Asl.INSTANCE.aslDestroyWindow(id);
        App.getInstance()._removeWindow(this);
    }

    public void addView(View v) {
        v.setWindow(this);
        contentView.addSubView(v);
    }

    public void move(double x, double y) {
        location.x = x;
        location.y = y;
        Asl.INSTANCE.aslMoveWindow(id, x, y);
    }

    public void bringWindowToFront() {
        Asl.INSTANCE.aslBringWindowToFront(id);
    }

    public void addMouseEventListener(MouseEventListener l) {
        mouseEventListeners.add(l);
    }

    public void addKeyEventListener(KeyEventListener l) {
        keyEventListeners.add(l);
    }

    public View getFocusedView() {
        return focusedView;
    }

    public void setFocusedView(View focusedView) {
        this.focusedView = focusedView;
    }

    boolean pointIsInsideResizeHotspot(double x, double y) {
        if (x >= getWidth() - 60 &&
            y >= getHeight() - 60 /*&&
            x <= getWidth() &&
            y <= getHeight()*/) {
            return true;
        }
        return false;
    }

    final void dispatchEvent(Event e) {
        if (e == null) return;

        if (e instanceof InputEvent) {
            InputEvent inputEvent = (InputEvent)e;
            if (inputEvent instanceof KeyEvent) {
                KeyEvent keyEvent = (KeyEvent)inputEvent;

                for (KeyEventListener l : keyEventListeners) {
                    l.keyEventOccurred(keyEvent);
                }

                if (focusedView != null) {
                    focusedView.dispatchKeyEvent(keyEvent);
                } else {
                    rootView.dispatchKeyEvent(keyEvent);
                }
            }
            else if (inputEvent instanceof MouseEvent) {
                MouseEvent mouseEvent = (MouseEvent)inputEvent;
                if (pointIsInsideResizeHotspot(mouseEvent.getLocation().x, mouseEvent.getLocation().y)) {
                    if (mouseEvent.getMouseEventType() == MouseEvent.MouseEventType.DRAG) {

                        double width = getWidth();
                        double newWidth = mouseEvent.getLocationInScreen().x - location.x;
                        if (newWidth >= MIN_WIDTH) {
                            width = newWidth;
                        }

                        double height = getHeight();
                        double newHeight = mouseEvent.getLocationInScreen().y - location.y - 40;
                        if (newHeight >= MIN_HEIGHT) {
                            height = newHeight;
                        }

                        // Check if there is a real change in the window size
                        if (width != getWidth() || height != getHeight()) {
                            resize(width, height);
                        }
                    }
                }

                for (MouseEventListener l : mouseEventListeners) {
                    l.mouseEventOccurred(mouseEvent);
                }

                rootView.dispatchMouseEvent(mouseEvent);
            }
        }
        else if (e instanceof WindowLocationChangedEvent) {
            WindowLocationChangedEvent windowLocationChangedEvent = (WindowLocationChangedEvent)e;
            location.x = windowLocationChangedEvent.getNewX();
            location.y = windowLocationChangedEvent.getNewY();
        }
    }

    View getRootView() {
        return rootView;
    }

    public long getId() {
        return id;
    }

    public double getWidth() {
        return contentFrame.size.width;
    }

    public double getHeight() {
        return contentFrame.size.height;
    }

    public double getX() {
        return location.x;
    }

    public double getY() {
        return location.y;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        if (title != null) {
            this.title = title;
            rootView.markAsNeedsRedraw();
        }
    }

    enum CloseButtonStatus {
        NORMAL,
        PRESSED
    }

    class CloseButton extends View implements View.DrawEventListener, View.MouseEventListener {

        private CloseButtonStatus status = CloseButtonStatus.NORMAL;


        public CloseButton(double x, double y, double width, double height) {
            super(x, y, width, height);

            addDrawEventListener(this);
            addMouseEventListener(this);
        }

        @Override
        public void draw(Graphics g) {
            g.setColor(new Color(3, 169, 244));
            g.fillRect(0, 0, (int)getWidth(), (int)getHeight());

            BufferedImage normalCloseButtonImage = Resources.get("close_btn_normal");
            BufferedImage pressedCloseButtonImage = Resources.get("close_btn_pressed");
            if (status == CloseButtonStatus.NORMAL) {
                g.drawImage(normalCloseButtonImage, 0, 0, null);
            }
            else if (status == CloseButtonStatus.PRESSED) {
                g.drawImage(pressedCloseButtonImage, 0, 0, null);
            }
        }

        @Override
        public void mouseEventOccurred(MouseEvent e) {
            if (e.getMouseButtonType() == MouseEvent.MouseButtonType.LEFT) {
                if (e.getMouseEventType() == MouseEvent.MouseEventType.PRESS) {
                    status = CloseButtonStatus.PRESSED;
                    markAsNeedsRedraw();
                }
                else if(e.getMouseEventType() == MouseEvent.MouseEventType.RELEASE) {
                    getWindow().destroy();
                }
            }
        }
    }

    class RootView extends View implements View.DrawEventListener {

        public RootView(double x, double y, double width, double height) {
            super(x, y, width, height);
            addDrawEventListener(this);
        }

        @Override
        public void draw(Graphics g) {
            Graphics2D g2d = (Graphics2D)g;

            NinePatch.render(g2d, "window", null, getWidth(), getHeight());

            if (title != null) {
                g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_LCD_HRGB);
                g2d.setColor(Color.WHITE);
                g2d.drawString(title, 45, 27);
            }
        }
    }

    class ContentView extends View implements View.DrawEventListener {

        ContentView(RectF frame) {
            super(frame);
            init();
        }

        public ContentView(double x, double y, double width, double height) {
            super(x, y, width, height);
            init();
        }

        void init() {
            addDrawEventListener(this);
        }

        @Override
        public void draw(Graphics g) {
            g.setColor(Color.WHITE);
            g.fillRect(0, 0, (int)getWidth(), (int)getHeight());
        }
    }

    public interface MouseEventListener {
        void mouseEventOccurred(MouseEvent e);
    }

    public interface KeyEventListener {
        void keyEventOccurred(KeyEvent e);
    }
}
