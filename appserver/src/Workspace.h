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

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

namespace appserver
{
    const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;
    
    class Workspace
    {
    public:
        Workspace(std::shared_ptr<Compositor> compositor);
        virtual ~Workspace();
        virtual void run() = 0;
        std::shared_ptr<InputSource> getInputSource() const;
        void setInputSource(std::shared_ptr<InputSource> inputSource);
        static void avoidBusyWait(const long nsec = INTERVAL_MS);
    protected:
        std::shared_ptr<InputSource> _inputSource;
        std::shared_ptr<Compositor> _compositor;
    };
}

#endif
