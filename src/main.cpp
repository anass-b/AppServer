//
//  main.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <Server.h>

using namespace appserver;

int main(int argc, char **argv)
{
    Server::getSingleton()->run(kBackendModeGLFW);
    
    return 0;
}