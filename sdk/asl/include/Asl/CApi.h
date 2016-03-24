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

extern "C" {
    
    typedef struct
    {
        int type;
        int button;
        double x;
        double y;
        double absX;
        double absY;
    } AslMouseEvent;

    typedef struct
    {
        int type;
        char *text;
        size_t textSize;
    } AslKeyEvent;
        
    typedef struct
    {
        int type;
        AslKeyEvent keyEvent;
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
        int type;
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
