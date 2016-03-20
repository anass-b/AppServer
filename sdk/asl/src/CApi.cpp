//
//  AppServer.cpp
//  appserverlib
//
//  Created by Anass on 2014-10-04.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <memory>
#include <Asl/Connector.h>
#include <Asl/CApi.h>

using namespace asl;

std::shared_ptr<Connector> gConnector = nullptr;

EXPORT
void aslInit()
{
    gConnector = std::make_shared<Connector>();
}

EXPORT
void aslSubscribe()
{
    gConnector->subscribe();
}

EXPORT
long aslNewWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, int rasterType, bool visible)
{
    return gConnector->newWindow(data, dataSize, x, y, width, height, rasterType, visible);
}

EXPORT
void aslUpdateWindowSurface(long windowId, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height)
{
    gConnector->updateWindowSurface(windowId, data, dataSize, x, y, width, height);
}

EXPORT
void aslResizeWindow(long windowId, unsigned char *data, unsigned long dataSize, double width, double height)
{
    gConnector->resizeWindow(windowId, data, dataSize, width, height);
}

EXPORT
void aslChangeWindowVisibility(long windowId, bool visible)
{
    gConnector->changeWindowVisiblity(windowId, visible);
}

EXPORT
void aslBringWindowToFront(long windowId)
{
    gConnector->bringWindowToFront(windowId);
}

EXPORT
void aslMoveWindow(long windowId, double x, double y)
{
    gConnector->moveWindow(windowId, x, y);
}

EXPORT
void aslDestroyWindow(long windowId)
{
    gConnector->destroyWindow(windowId);
}

EXPORT
AslEvent aslWaitEvent()
{
    AslEvent event;
    
    event.type = -1;
    event.windowId = -1;
    event.inputEvent.type = -1;
    event.inputEvent.keyEvent.type = -1;
    event.inputEvent.keyEvent.charCode = -1;
    event.inputEvent.mouseEvent.type = -1;
    event.inputEvent.mouseEvent.x = -1;
    event.inputEvent.mouseEvent.y = -1;
    event.inputEvent.mouseEvent.absX = -1;
    event.inputEvent.mouseEvent.absY = -1;
    event.inputEvent.mouseEvent.button = -1;
    
    std::shared_ptr<asl::Event> aslEvent = gConnector->waitEvent();

    if (aslEvent != nullptr && aslEvent->getEventType() == asl::kEventTypeInput) {
        std::shared_ptr<asl::InputEvent> aslInputEvent = std::dynamic_pointer_cast<asl::InputEvent>(aslEvent);
        if (aslInputEvent != nullptr) {
            event.type = aslInputEvent->getEventType();
            event.windowId = aslInputEvent->getWindowId();
            
            if (aslInputEvent->getInputEventType() == asl::kInputEventTypeMouse) {
                std::shared_ptr<asl::MouseEvent> aslMouseEvent = std::dynamic_pointer_cast<asl::MouseEvent>(aslInputEvent);
                
                event.inputEvent.type = kInputEventTypeMouse;
                
                event.inputEvent.mouseEvent.type = aslMouseEvent->getMouseEventType();
                event.inputEvent.mouseEvent.button = aslMouseEvent->getMouseButton();
                event.inputEvent.mouseEvent.x = aslMouseEvent->getX();
                event.inputEvent.mouseEvent.y = aslMouseEvent->getY();
                event.inputEvent.mouseEvent.absX = aslMouseEvent->getAbsX();
                event.inputEvent.mouseEvent.absY = aslMouseEvent->getAbsY();
            }
            else if (aslInputEvent->getInputEventType() == asl::kInputEventTypeKey) {
                std::shared_ptr<asl::KeyEvent> aslKeyEvent = std::dynamic_pointer_cast<asl::KeyEvent>(aslInputEvent);
                
                event.inputEvent.type = (int)kInputEventTypeKey;
                
                event.inputEvent.keyEvent.type = kInputEventTypeKey;
                event.inputEvent.keyEvent.charCode = aslKeyEvent->getCharCode();
            }
        }
    }
    else if (aslEvent != nullptr && aslEvent->getEventType() == asl::kEventTypeWindowLocationChanged) {
        std::shared_ptr<asl::WindowLocationChangedEvent> aslWindowLocationChangedEvent = std::dynamic_pointer_cast<asl::WindowLocationChangedEvent>(aslEvent);
        if (aslWindowLocationChangedEvent != nullptr) {
            event.type = aslWindowLocationChangedEvent->getEventType();
            event.windowId = aslEvent->getWindowId();
            event.windowLocationChangedEvent.newX = aslWindowLocationChangedEvent->getNewWindowX();
            event.windowLocationChangedEvent.newY = aslWindowLocationChangedEvent->getNewWindowY();
        }
    }
    
    return event;
}

