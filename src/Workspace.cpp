//
//  Screen.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <Workspace.h>

using namespace appserver;

Workspace::Workspace(std::shared_ptr<Compositor> compositor)
{
    _compositor = compositor;
}

std::shared_ptr<InputSource> Workspace::getInputSource() const
{
    return _inputSource;
}

void Workspace::setInputSource(std::shared_ptr<InputSource> inputSource)
{
    _inputSource = inputSource;
}

Workspace::~Workspace()
{
}

