//
//  GLCompositor.c
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <GLCompositor.h>
#include <GLFW/glfw3.h>
#include <memory>

using namespace appserver;
 
GLCompositor::GLCompositor()
{
}

void GLCompositor::compose()
{
    int r = 65;
    int g = 105;
    int b = 170;
    glClearColor(r/255.0f, g/255.0f, b/255.0f, 0.0);
    
    // This sets up the viewport so that the coordinates (0, 0) are at the top left of the window
    glViewport(0, 0, _size.width, _size.height);
    
    
    // Set up the orthographic projection so that coordinates (0, 0) are in the top left
    // and the minimum and maximum depth is -10 and 10. To enable depth just put in
    // glEnable(GL_DEPTH_TEST)
     
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, _size.width, _size.height, 0, -10, 10);
    
    // Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    
    // Clear the screen and depth
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (auto &it : _windows) {
        std::shared_ptr<Window> w(it);
        if (w != nullptr) {
            w->performOperationsAndDraw();
        }
    }
}

GLCompositor::~GLCompositor()
{
}

