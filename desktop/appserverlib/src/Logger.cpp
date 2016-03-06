//
//  Logger.cpp
//  appserver
//
//  Created by Anass on 15/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <Asl/Logger.h>

//#define ENABLE_LOGS 1

using namespace asl;

bool Logger::_enabled = true;

void Logger::setEnabled(bool enabled)
{
    _enabled = enabled;
}

void Logger::log(const string& msg)
{
    log("", msg);
}

void Logger::log(const string& tag, const string& msg)
{
    if (!_enabled) return;
    
    if (tag.length() == 0) {
        cout << msg << endl;
    }
    else {
        cout << tag << " | " << msg << endl;
    }
}

bool Logger::isEnabled() {
    return _enabled;
}