//
//  Screen.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <iostream>
#include <memory>

#include <InputSource.h>
#include <Compositor.h>

namespace appserver
{
    class Workspace
    {
    public:
        Workspace(std::shared_ptr<Compositor> compositor);
        virtual ~Workspace();
        virtual void run() = 0;
        std::shared_ptr<InputSource> getInputSource() const;
        void setInputSource(std::shared_ptr<InputSource> inputSource);
    protected:
        std::shared_ptr<InputSource> _inputSource;
        std::shared_ptr<Compositor> _compositor;
    };
}

#endif
