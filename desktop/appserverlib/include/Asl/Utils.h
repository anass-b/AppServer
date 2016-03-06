//
//  Utils.h
//  appserverlib
//
//  Created by Anass on 2014-10-04.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserverlib_Utils_h
#define appserverlib_Utils_h

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

namespace asl
{
    const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;
    
    void avoidBusyWait(const long nsec = INTERVAL_MS);
}

#endif
