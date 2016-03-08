package com.medar.coffeecode.app;

import com.medar.coffeecode.graphics.PointF;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public final class App implements Runnable {

    private static App instance = new App();
    private CopyOnWriteArrayList<Window> windows;
    private ArrayList<AppListener> listeners;
    private boolean runLoop = true;
    private boolean quitAppIfLastWindowDestroyed;

    private App() {
        windows = new CopyOnWriteArrayList<Window>();
        listeners = new ArrayList<AppListener>();
    }

    /*
     * TODO: Find a way to prohibit the users from calling this method explicitly
     */
    @Override
    public final void run() {
        while (runLoop) {
            for (Window window: windows) {
                if (window.getRootView().needsRedraw()) {
                    window.flush();
                }
            }
        }
    }

    public void start() {
        // Connect and subscribe to the AppServer
        Asl.INSTANCE.aslInit();
        Asl.INSTANCE.aslSubscribe();

        Thread thread = new Thread(this);
        thread.start();

        for (AppListener listener : listeners) {
            listener.appStarted();
        }

        while (runLoop) {
            Event event = parseAslEvent(Asl.INSTANCE.aslWaitEvent());
            if (event != null) {
                for (Window window : windows) {
                    if (window.getId() == event.getWindowId()) {
                        window.dispatchEvent(event);
                        break;
                    }
                }
            }
        }
    }

    public final void start(AppListener listener) {
        addApplicationListener(listener);
        start();
    }

    private Event parseAslEvent(Asl.AslEvent.ByValue aslEvent) {
        if (aslEvent.type == Asl.EVENT_TYPE_INPUT) {
            if (aslEvent.inputEvent.type == Asl.INPUT_EVENT_TYPE_MOUSE) {
                MouseEvent.MouseEventType mouseEventType = MouseEvent.MouseEventType.UNDEFINED;
                switch (aslEvent.inputEvent.mouseEvent.type) {
                    case Asl.MOUSE_EVENT_TYPE_PRESS:
                        mouseEventType = MouseEvent.MouseEventType.PRESS;
                        break;
                    case Asl.MOUSE_EVENT_TYPE_RELEASE:
                        mouseEventType = MouseEvent.MouseEventType.RELEASE;
                        break;
                    case Asl.MOUSE_EVENT_TYPE_MOVE:
                        mouseEventType = MouseEvent.MouseEventType.MOVE;
                        break;
                    case Asl.MOUSE_EVENT_TYPE_DRAG:
                        mouseEventType = MouseEvent.MouseEventType.DRAG;
                        break;
                }

                MouseEvent.MouseButtonType mouseButtonType = MouseEvent.MouseButtonType.UNDEFINED;
                switch (aslEvent.inputEvent.mouseEvent.button) {
                    case Asl.MOUSE_BUTTON_RIGHT:
                        mouseButtonType = MouseEvent.MouseButtonType.RIGHT;
                        break;
                    case Asl.MOUSE_BUTTON_LEFT:
                        mouseButtonType = MouseEvent.MouseButtonType.LEFT;
                        break;
                    case Asl.MOUSE_BUTTON_MIDDLE:
                        mouseButtonType = MouseEvent.MouseButtonType.MIDDLE;
                        break;
                }

                PointF location = new PointF(aslEvent.inputEvent.mouseEvent.x, aslEvent.inputEvent.mouseEvent.y);
                PointF locationInScreen = new PointF(aslEvent.inputEvent.mouseEvent.absX, aslEvent.inputEvent.mouseEvent.absY);

                return new MouseEvent(mouseEventType, mouseButtonType, aslEvent.windowId, -1, -1, location, locationInScreen);
            }
            else if (aslEvent.inputEvent.type == Asl.INPUT_EVENT_TYPE_KEY) {
                KeyEvent.KeyEventType keyEventType = KeyEvent.KeyEventType.UNDEFINED;
                switch (aslEvent.inputEvent.keyEvent.type) {
                    case Asl.KEY_EVENT_TYPE_PRESS:
                        keyEventType = KeyEvent.KeyEventType.PRESS;
                        break;
                    case Asl.KEY_EVENT_TYPE_RELEASE:
                        keyEventType = KeyEvent.KeyEventType.RELEASE;
                        break;
                }

                return new KeyEvent(keyEventType, aslEvent.windowId, -1, -1, (char)aslEvent.inputEvent.keyEvent.charCode);
            }
        }
        else if (aslEvent.type == Asl.EVENT_TYPE_WINDOW_LOCATION_CHANGED) {
            return new WindowLocationChangedEvent(
                    aslEvent.windowId,
                    aslEvent.windowLocationChangedEvent.newX,
                    aslEvent.windowLocationChangedEvent.newY);
        }

        return null;
    }

    public void quit() {
        runLoop = false;
    }

    public static App getInstance() {
        return instance;
    }

    public final void addApplicationListener(AppListener listener) {
        listeners.add(listener);
    }

    public void setQuitAppIfLastWindowDestroyed(boolean quitAppIfLastWindowDestroyed) {
        this.quitAppIfLastWindowDestroyed = quitAppIfLastWindowDestroyed;
    }

    final void addWindow(Window w) {
        windows.add(w);
    }

    public final void _removeWindow(Window w) {
        windows.remove(w);
        if (quitAppIfLastWindowDestroyed && windows.size() == 0) {
            quit();
        }
    }

    final List<Window> getWindows() {
        return windows;
    }

    public interface AppListener {
        void appStarted();
        void appQuitting();
    }
}
