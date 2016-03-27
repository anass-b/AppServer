#ifndef PROTOCOL_H
#define PROTOCOL_H

/*
 * Request IDs (Sent by client, received by server)
 */
#define AspRequestUndefined             0
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
#define AspWindowRasterUndefined    0
#define AspWindowRasterRGBA         1
#define AspWindowRasterARGB         2

/*
 * Event types
 */
#define AspEventTypeUndefined       0
#define AspEventTypeMouseMove       1
#define AspEventTypeMouseButton     2
#define AspEventTypeMouseScroll     3
#define AspEventTypeKey             4
#define AspEventTypeText            5
#define AspEventTypeQuit            6

/*
 * Mouse buttons
 */
#define AspMouseButtonUndefined     0
#define AspMouseButtonRight         1
#define AspMouseButtonLeft          2
#define AspMouseButtonMiddle        3

/*
 * Mouse button states
 */
#define AspMouseButtonStateUndefined    0
#define AspMouseButtonStatePressed      1
#define AspMouseButtonStateReleased     2

/*
 * Key event states
 */
#define AspKeyStateUndefined    0
#define AspKeyStatePressed      1
#define AspKeyStateReleased     2

/*
 * Port values
 */
#define AspReqListenerThreadPortValue 20000

typedef uint32_t TAppId;
typedef pid_t TProcId;
typedef uint32_t TWindowId;
typedef uint32_t TWindowZ;

struct Asp_SubscribeRequest
{
    TProcId pid;
    TAppId clientId;
};

struct Asp_Request
{
    TAppId clientId;
    uint8_t type;
    TWindowId winId;
    double field0;
    double field1;
    double field2;
    double field3;
    int8_t field4;
    int8_t field5;
    uint64_t dataSize;
};

struct Asp_Event {
    TWindowId winId;
    uint8_t type;
    double field0;
    double field1;
    double field2;
    double field3;
    uint8_t field4;
    uint8_t field5;
    uint32_t timestamp;
};

typedef struct Asp_Event Asp_Event;
typedef struct Asp_Request Asp_Request;
typedef struct Asp_SubscribeRequest Asp_SubscribeRequest;

#endif
