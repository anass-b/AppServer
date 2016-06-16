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
#include <QApplication>

using namespace appserver;

int main(int argc, char** argv)
{
    std::string appsHost = argc > 1 ? argv[1] : "localhost";

    QApplication app(argc, argv);

    Server::getSingleton()->setAppsHost(appsHost);
    Server::getSingleton()->run(argc, argv);

    app.exec();

    return 0;
}
