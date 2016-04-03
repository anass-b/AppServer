//
//  appserver.h
//  appserverlib
//
//  Created by Anass on 2014-10-04.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef ASL_H
#define ASL_H

#ifdef _WIN32
#define EXPORT __declspec( dllexport )
#else
#define EXPORT __attribute__((visibility("default")))
#endif

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

EXPORT void* aslCreateContext();
EXPORT void aslSubscribe(void *context);
EXPORT uint32_t aslCreateWindow(void *context, void *data, uint64_t dataSize, double x, double y, double width, double height, uint8_t rasterType);
EXPORT void aslUpdateWindow(void *context, uint32_t windowId, void *data, uint64_t dataSize, double x, double y, double width, double height, bool compression);
EXPORT void aslResizeWindow(void *context, uint32_t windowId, void *data, uint64_t dataSize, double width, double height);
EXPORT void aslChangeWindowVisibility(void *context, uint32_t windowId, bool visible);
EXPORT void aslBringWindowToFront(void *context, uint32_t windowId);
EXPORT void aslMoveWindow(void *context, uint32_t windowId, double x, double y);
EXPORT void aslDestroyWindow(void *context, uint32_t windowId);
EXPORT AslEvent aslWaitEvent(void *context);

}

#endif
