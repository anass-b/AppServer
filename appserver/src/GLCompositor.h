//
//  GLCompositor.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef GLCOMPOSITOR_H
#define GLCOMPOSITOR_H

#include <memory>

#include <Compositor.h>

namespace appserver
{
    class GLCompositor : public Compositor
    {
    public:
        GLCompositor();
        virtual ~GLCompositor();
        virtual void compose();
    };
}

#endif
