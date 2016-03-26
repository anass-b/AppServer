//
//  GlfwInputSource.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef SDLINPUTSOURCE_H
#define SDLINPUTSOURCE_H

#include <memory>
#include <eventsource.h>

namespace appserver
{
    class SDLEventSource : public EventSource
    {
    public:
        SDLEventSource();
        virtual std::shared_ptr<Event> pollEvent();
    private:
        double _mousePosX = 0;
        double _mousePosY = 0;
        bool _mouseHeld = false;
    };
}

#endif
