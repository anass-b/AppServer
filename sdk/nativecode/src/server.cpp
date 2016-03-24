#include <private/server.h>

using namespace app;

Server::Server()
{
    mConnector = std::make_shared<asl::Connector>();
    mConnector->subscribe();
}

int Server::createWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, bool visible)
{
    try {
        return (int)mConnector->newWindow(data, dataSize, x, y, width, height, AspWindowRasterARGB, visible);
    } catch (std::exception e) {
        return -1;
    }
    return -1;
}

int Server::updateWindowSurface(TWindowId wndId, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height)
{
    try {
        mConnector->updateWindowSurface(wndId, data, dataSize, x, y, width, height);
    } catch (std::exception e) {
        return 1;
    }
    return 0;
}

int Server::resizeWindow(TWindowId wndId, unsigned char *data, unsigned long dataSize, double width, double height)
{
    try {
        mConnector->resizeWindow(wndId, data, dataSize, width, height);
    } catch (std::exception e) {
        return 1;
    }
    return 0;
}

int Server::setWindowVisibility(TWindowId wndId, bool visible)
{
    try {
        mConnector->changeWindowVisiblity(wndId, visible);
    } catch (std::exception e) {
        return 1;
    }
    return 0;
}

int Server::bringWindowToFront(TWindowId wndId)
{
    try {
        mConnector->bringWindowToFront(wndId);
    } catch (std::exception e) {
        return 1;
    }
    return 0;
}

int Server::moveWindow(TWindowId wndId, double x, double y)
{
    try {
        mConnector->moveWindow(wndId, x, y);
    } catch (std::exception e) {
        return 1;
    }
    return 1;
}

InputEvent* Server::waitInputEvent()
{
    std::shared_ptr<asl::Event> aslEvent(mConnector->waitEvent());
    InputEvent *inputEvent = new InputEvent;
    if (aslEvent != nullptr && aslEvent->getEventType() == asl::kEventTypeInput) {
        std::shared_ptr<asl::InputEvent> aslInputEvent = std::dynamic_pointer_cast<asl::InputEvent>(aslEvent);
        
        if (aslInputEvent != nullptr) {
            //inputEvent->type = (InputEventType)aslInputEvent->getEventType();
            inputEvent->winId = (unsigned int)aslInputEvent->getWindowId();
            
            if (aslInputEvent->getInputEventType() == asl::kInputEventTypeMouse) {
                std::shared_ptr<asl::MouseEvent> aslMouseEvent = std::dynamic_pointer_cast<asl::MouseEvent>(aslInputEvent);
                
                inputEvent->type = kInputEventTypeMouse;
                inputEvent->keyEvent = nullptr;
                
                MouseEvent *mouseEvent = new MouseEvent;
                mouseEvent->winId = (unsigned int)aslMouseEvent->getWindowId();
                mouseEvent->type = (MouseEventType)aslMouseEvent->getMouseEventType();
                mouseEvent->button = (MouseButton)aslMouseEvent->getMouseButton();
                mouseEvent->x = aslMouseEvent->getX();
                mouseEvent->y = aslMouseEvent->getY();
                mouseEvent->absX = aslMouseEvent->getAbsX();
                mouseEvent->absY = aslMouseEvent->getAbsY();
                mouseEvent->viewId = -1;
                mouseEvent->viewTag = -1;
                
                inputEvent->mouseEvent = mouseEvent;
            }
            else if (aslInputEvent->getInputEventType() == asl::kInputEventTypeKey) {
                std::shared_ptr<asl::KeyEvent> aslKeyEvent = std::dynamic_pointer_cast<asl::KeyEvent>(aslInputEvent);
                
                inputEvent->type = kInputEventTypeKey;
                inputEvent->mouseEvent = nullptr;
                
                KeyEvent *keyEvent = new KeyEvent;
                keyEvent->winId = (unsigned int)aslKeyEvent->getWindowId();
                keyEvent->type = (KeyEventType)aslKeyEvent->getKeyEventType();
                keyEvent->keyCode = 0;
                keyEvent->charCode = aslKeyEvent->getCharCode();
                keyEvent->viewId = -1;
                keyEvent->viewTag = -1;
                
                inputEvent->keyEvent = keyEvent;
            }
        }
    }
    return inputEvent;
}

int Server::unregisterClient()
{
    mConnector->unsubscribe();
    return 0;
}


