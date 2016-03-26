//
//  appserver.h
//  appserverlib
//
//  Created by Anass on 2014-10-04.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserverlib_appserver_h
#define appserverlib_appserver_h

#define EXPORT __attribute__((visibility("default")))

#include <stdint.h>
#include <stdbool.h>
#include <asl/keycode.h> // This includes <asl/scancode.h>

extern "C" {
    
    typedef struct
    {
        int8_t type;
        int8_t button;
        double x;
        double y;
        double absX;
        double absY;
        int32_t scrollX;
        int32_t scrollY;
    } AslMouseEvent;

    typedef struct
    {
        int8_t type;
        int32_t scancode;
        int32_t keycode;
        int32_t keymod;
        int8_t state;
        bool repeat;
    } AslKeyEvent;
    
    typedef struct
    {
        char *text;
        size_t textSize;
    } AslTextEvent;
        
    typedef struct
    {
        int8_t type;
        AslKeyEvent keyEvent;
        AslTextEvent textEvent;
        AslMouseEvent mouseEvent;
    } AslInputEvent;
    
    typedef struct
    {
        double newX;
        double newY;
    } AslWindowLocationChangedEvent;
    
    typedef struct
    {
        long windowId;
        int8_t type;
        AslInputEvent inputEvent;
        AslWindowLocationChangedEvent windowLocationChangedEvent;
    } AslEvent;

    void aslInit();
    void aslSubscribe();
    long aslNewWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, int rasterType, bool visible);
    void aslUpdateWindowSurface(long windowId, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height);
    void aslResizeWindow(long windowId, unsigned char *data, unsigned long dataSize, double width, double height);
    void aslChangeWindowVisibility(long windowId, bool visible);
    void aslBringWindowToFront(long windowId);
    void aslMoveWindow(long windowId, double x, double y);
    void aslDestroyWindow(long windowId);
    AslEvent aslWaitEvent();

}

#endif
