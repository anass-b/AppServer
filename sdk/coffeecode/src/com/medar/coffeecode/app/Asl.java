package com.medar.coffeecode.app;


import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import java.util.Arrays;
import java.util.List;

interface Asl extends Library {

    Asl INSTANCE = (Asl) Native.loadLibrary(("appserver"), Asl.class);

    public static final int MOUSE_EVENT_TYPE_UNDEFINED = 0;
    public static final int MOUSE_EVENT_TYPE_PRESS = 1;
    public static final int MOUSE_EVENT_TYPE_RELEASE = 2;
    public static final int MOUSE_EVENT_TYPE_MOVE = 3;
    public static final int MOUSE_EVENT_TYPE_DRAG = 4;

    public static final int MOUSE_BUTTON_UNDEFINED = 0;
    public static final int MOUSE_BUTTON_RIGHT = 1;
    public static final int MOUSE_BUTTON_LEFT = 2;
    public static final int MOUSE_BUTTON_MIDDLE = 3;

    public static final int KEY_EVENT_TYPE_UNDEFINED = 0;
    public static final int KEY_EVENT_TYPE_PRESS = 1;
    public static final int KEY_EVENT_TYPE_RELEASE = 2;

    public static final int INPUT_EVENT_TYPE_UNDEFINED = 0;
    public static final int INPUT_EVENT_TYPE_MOUSE = 1;
    public static final int INPUT_EVENT_TYPE_KEY = 2;

    public static final int EVENT_TYPE_UNDEFINED = 0;
    public static final int EVENT_TYPE_INPUT = 1;
    public static final int EVENT_TYPE_WINDOW_LOCATION_CHANGED = 2;

    public static final int WINDOW_RASTER_TYPE_RGBA = 1;
    public static final int WINDOW_RASTER_TYPE_ARGB = 2;

    static class AslMouseEvent extends Structure implements Structure.ByValue
    {
        public static class ByValue extends AslMouseEvent implements Structure.ByValue {}
        public int type;
        public int button;
        public double x;
        public double y;
        public  double absX;
        public double absY;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[]{"type", "button", "x", "y", "absX", "absY"});
        }
    }

    static class AslKeyEvent extends Structure implements Structure.ByValue
    {
        public static class ByValue extends AslKeyEvent implements Structure.ByValue {}
        public int type;
        public int charCode;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[]{"type", "charCode"});
        }
    }

    static class AslInputEvent extends Structure implements Structure.ByValue
    {
        public static class ByValue extends AslInputEvent implements Structure.ByValue {}
        public int type;
        public AslKeyEvent keyEvent;
        public AslMouseEvent mouseEvent;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[]{"type", "keyEvent", "mouseEvent"});
        }
    }

    static class AslWindowLocationChangedEvent extends Structure implements Structure.ByValue
    {
        public static class ByValue extends AslWindowLocationChangedEvent implements Structure.ByValue {}
        public double newX;
        public double newY;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[]{"newX", "newY"});
        }
    }

    static class AslEvent extends Structure implements Structure.ByValue
    {
        public static class ByValue extends AslEvent implements Structure.ByValue {}
        public long windowId;
        public int type;
        public AslInputEvent inputEvent;
        public AslWindowLocationChangedEvent windowLocationChangedEvent;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[]{"windowId", "type", "inputEvent", "windowLocationChangedEvent"});
        }
    }

    void aslInit();
    void aslSubscribe();
    long aslNewWindow(Pointer data, long dataSize, double x, double y, double width, double height, int rasterType, boolean visible);
    void aslUpdateWindowSurface(long windowId, Pointer data, long dataSize, double x, double y, double width, double height);
    void aslResizeWindow(long windowId, Pointer data, long dataSize, double width, double height);
    void aslChangeWindowVisibility(long windowId, boolean visible);
    void aslBringWindowToFront(long windowId);
    void aslMoveWindow(long windowId, double x, double y);
    void aslDestroyWindow(long windowId);
    AslEvent.ByValue aslWaitEvent();
}


