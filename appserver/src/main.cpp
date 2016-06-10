//
//  main.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <string>
#include <server.h>

using namespace appserver;

int main(int argc, char** argv)
{
    std::string appsHost = argc > 1 ? argv[1] : "localhost";

    Server::getSingleton()->setAppsHost(appsHost);
    Server::getSingleton()->run();

    return 0;
}
