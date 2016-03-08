//
//  Logger.h
//  appserver
//
//  Created by Anass on 15/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_Logger_h
#define appserver_Logger_h

#include <iostream>
#include <string>

using namespace std;

namespace asl
{
    class Logger
    {
    public:
        static void setEnabled(bool enabled);
        static void log(const string& msg);
        static void log(const string& tag, const string& msg);
        static bool isEnabled();
    private:
        static bool _enabled;
    };
}

#endif
