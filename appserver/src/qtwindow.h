#ifndef QTWINDOW_H
#define QTWINDOW_H

#include <memory>
#include <QWindow>
#include <window.h>

namespace appserver {

class QtWindow : public QWindow, public Window {
public:
    QtWindow(TWindowId id, const Rect& frame, int rasterType, bool visible = true);
    virtual ~QtWindow();

    virtual void performOperationsAndDraw();
    virtual bool operationsFinished();

    virtual void move(Point location);
    virtual void create(void* pixels, size_t bytes);
    virtual void resize(void* pixels, size_t bytes);
    virtual void updatePixels(void* pixels, size_t bytes, const Rect& dirtyRect);
};
}

#endif
