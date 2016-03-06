//
//  GLCompositor.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_GLCompositor_h
#define appserver_GLCompositor_h

#include <memory>

#include <Compositor.h>

namespace appserver
{
    class HCompositor : public Compositor
    {
    public:
        HCompositor();
        virtual ~HCompositor();
        virtual void compose();
    };
}

#endif
