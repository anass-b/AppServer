//
//  GlfwScreen.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <GlfwWorkspace.h>
#include <GlfwInputSource.h>
#include <Asl/Utils.h>

using namespace appserver;
 
GlfwWorkspace::GlfwWorkspace(std::shared_ptr<Compositor> compositor, unsigned int width, unsigned int height) : Workspace(compositor)
{
    if (!glfwInit()) {
        throw std::exception();
    }
    
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height cannot be negative or zero");
    }
    
    _window = glfwCreateWindow(width, height, "appserver", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        throw std::exception();
    }
    
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwMakeContextCurrent(_window);
}

void GlfwWorkspace::run()
{
    _inputSource = std::make_shared<GlfwInputSource>(shared_from_this());
    std::shared_ptr<GlfwInputSource> glfwInputSrc = std::dynamic_pointer_cast<GlfwInputSource>(_inputSource);
    
    while (!glfwWindowShouldClose(_window)) {
        float ratio;
        int width, height;
        
        glfwGetFramebufferSize(_window, &width, &height);
        ratio = width / (float) height;
        
        _compositor->setSize(makeSize(width, height));
        _compositor->compose();
        
        glfwSwapBuffers(_window);
        
        glfwInputSrc->pollEvents();
        
        asl::avoidBusyWait(10 * NANO_SECOND_MULTIPLIER);
    }
}

GlfwWorkspace::~GlfwWorkspace()
{
    glfwDestroyWindow(_window);
}

