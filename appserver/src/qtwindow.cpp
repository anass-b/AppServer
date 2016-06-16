#include <qtwindow.h>
#include <memory>

using namespace appserver;

QtWindow::QtWindow(TWindowId id, const Rect& frame, int rasterType, bool visible)
    : appserver::Window(id, frame, rasterType, visible)
{

}

QtWindow::~QtWindow()
{
}

void QtWindow::move(Point location)
{
}

void QtWindow::create(void* pixels, size_t bytes)
{
}

void QtWindow::resize(void* pixels, size_t bytes)
{
}

void QtWindow::updatePixels(void* pixels, size_t bytes, const Rect& dirtyRect)
{
}

void QtWindow::performOperationsAndDraw()
{
}

bool QtWindow::operationsFinished()
{
    return true;
}
