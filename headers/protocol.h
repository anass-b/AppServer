#ifndef ASL_PROTOCOL_H
#define ASL_PROTOCOL_H

/*
 * Request IDs (Sent by client, received by server)
 */
#define AspRequestRegister              1
#define AspRequestUnregister            2
#define AspRequestCreateWindow          3
#define AspRequestMoveWindow            4
#define AspRequestResizeWindow          5
#define AspRequestMoveAndResizeWindow   6
#define AspRequestUpdateWindowSurface   7
#define AspRequestSetWindowVisibility   8
#define AspRequestDestroyWindow         9
#define AspRequestBringWindowToFront    10

#define AspUndefinedWindowId 0
#define AspUndefinedClientId 0

/*
 * Window Raster Type
 */
#define AspWindowRasterRGBA 1
#define AspWindowRasterARGB 2

/*
 * Event IDs (Sent by server, received by client)
 */
#define AspEventMouseInput              1
#define AspEventKeyInput                2
#define AspEventWindowLocationChanged   3
#define AspEventWindowResizeCompleted   4

/*
 * Mouse buttons
 */
#define AspMouseButtonRight     1
#define AspMouseButtonLeft      2
#define AspMouseButtonMiddle    3

/*
 * Mouse events
 */
#define AspMouseEventPress      1
#define AspMouseEventRelease    2
#define AspMouseEventMove       3
#define AspMouseEventDrag       4

/*
 * Key events
 */
#define AspKeyEventPress    1
#define AspKeyEventRelease  2

/*
 * Port values
 */
#define AspReqListenerThreadPortValue 20000

typedef unsigned long int TAppId;
typedef pid_t TProcId;
typedef unsigned long int TWindowId;
typedef unsigned long int TWindowZ;

struct Asp_SubscribeRequest
{
    TProcId pid;
    TAppId clientId;
};

struct Asp_Request
{
    TAppId clientId;
    unsigned int type;
    TWindowId winId;
    double field0;
    double field1;
    double field2;
    double field3;
    double field4;
    double field5;
    unsigned long dataSize;
};

struct Asp_Event {
    TWindowId winId;
    unsigned int type;
    double field0;
    double field1;
    double field2;
    double field3;
    double field4;
    unsigned int field5;
};

typedef struct Asp_Event Asp_Event;
typedef struct Asp_Request Asp_Request;
typedef struct Asp_SubscribeRequest Asp_SubscribeRequest;

#endif
