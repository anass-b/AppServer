#ifndef _MANGO_SERVER_H
#define _MANGO_SERVER_H

#include <iostream>
#include <memory>
#include <private/server.h>
#include <nativecode/input.h>
#include <Asl/Connector.h>

namespace app
{
    class Server
    {
    public:
        Server();
        int registerClient();
        int unregisterClient();
        int createWindow(unsigned char *data, unsigned long dataSize, double x, double y, double width, double height, bool visible);
        int updateWindowSurface(TWindowId wndId, unsigned char *data, unsigned long dataSize, double x, double y, double width, double height);
        int resizeWindow(TWindowId wndId, unsigned char *data, unsigned long dataSize, double width, double height);
        int setWindowVisibility(TWindowId wndId, bool visible);
        int bringWindowToFront(TWindowId wndId);
        int moveWindow(TWindowId wndId, double x, double y);
        int createInputEventListener();
        InputEvent* waitInputEvent();
    private:
        std::shared_ptr<asl::Connector> mConnector;
    };
}

#endif

