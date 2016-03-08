//
//  GlfwScreen.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef GLFWWORKSPACE_H
#define GLFWWORKSPACE_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <Workspace.h>

namespace appserver
{
    class GlfwWorkspace : public Workspace, public std::enable_shared_from_this<GlfwWorkspace>
    {
    public:
        friend class GlfwInputSource;
        GlfwWorkspace(std::shared_ptr<Compositor> compositor, unsigned int width = 1600, unsigned int height = 1000);
        virtual ~GlfwWorkspace();
        virtual void run();
    private:
        GLFWwindow *_window;
    };
}

#endif
