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

/*
 * Window Raster Type
 */
#define AslWindowRasterUndefined    0
#define AslWindowRasterRGBA         1
#define AslWindowRasterARGB         2

/*
 * Event types
 */
#define AslEventTypeUndefined       0
#define AslEventTypeMouseMove       1
#define AslEventTypeMouseButton     2
#define AslEventTypeMouseScroll     3
#define AslEventTypeKey             4
#define AslEventTypeText            5
#define AslEventTypeQuit            6

/*
 * Mouse buttons
 */
#define AslMouseButtonUndefined     0
#define AslMouseButtonRight         1
#define AslMouseButtonLeft          2
#define AslMouseButtonMiddle        3

/*
 * Mouse button states
 */
#define AslMouseButtonStateUndefined    0
#define AslMouseButtonStatePressed      1
#define AslMouseButtonStateReleased     2

/*
 * Key event states
 */
#define AslKeyStateUndefined    0
#define AslKeyStatePressed      1
#define AslKeyStateReleased     2

extern "C" {
    
typedef struct
{
    uint8_t buttonState;
    uint8_t button;
    double x;
    double y;
    double absX;
    double absY;
    int32_t scrollX;
    int32_t scrollY;
} AslMouseEvent;

typedef struct
{
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
    uint32_t windowId;
    uint8_t type;
    AslKeyEvent keyEvent;
    AslTextEvent textEvent;
    AslMouseEvent mouseEvent;
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
