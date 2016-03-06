//
//  Utils.cpp
//  appserverlib
//
//  Created by Anass on 2014-10-04.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>

namespace asl
{
    void avoidBusyWait(const long nsec)
    {
        timespec tim;
        tim.tv_sec  = 0;
        tim.tv_nsec = nsec;
        nanosleep(&tim, NULL);
    }
}

