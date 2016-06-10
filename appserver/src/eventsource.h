//
//  InputSource.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef EVENTSOURCE_H
#define EVENTSOURCE_H

#include <iostream>
#include <memory>
#include <events.h>

namespace appserver {

class EventSource {
public:
    virtual std::shared_ptr<Event> pollEvent() = 0;
};
}

#endif
