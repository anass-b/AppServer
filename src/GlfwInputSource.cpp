//
//  GlfwInputSource.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <GlfwInputSource.h>
#include <GlfwWorkspace.h>
#include <Server.h>
#include <Compositor.h>

using namespace appserver;

double GlfwInputSource::MousePosX = 0;
double GlfwInputSource::MousePosY = 0;
bool GlfwInputSource::MouseHeld = false;

GlfwInputSource::GlfwInputSource(std::shared_ptr<Workspace> screen) : InputSource(screen)
{
    glfwSetErrorCallback(GlfwInputSource::ErrorCallback);
    
    std::shared_ptr<GlfwWorkspace> workspace = dynamic_pointer_cast<GlfwWorkspace>(_workspace);
    
    glfwSetWindowUserPointer(workspace->_window, this);
    glfwSetKeyCallback(workspace->_window, KeyCallback);
    glfwSetCharCallback(workspace->_window, CharCallback);
    glfwSetCursorPosCallback(workspace->_window, CursorCallback);
    glfwSetMouseButtonCallback(workspace->_window, MouseButtonCallback);
}

bool GlfwInputSource::pollEvents()
{
    glfwPollEvents();
    return true;
}

void GlfwInputSource::onMouseMoveEvent(double x, double y)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    if (MouseHeld) {
        wm->onMouseDragEvent(makePoint(x, y));
    }
    else {
        wm->onMouseMoveEvent(makePoint(x, y));
    }
}

void GlfwInputSource::onMouseButtonEvent(double x, double y, int button, int type)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    
    if (type == AspMouseEventPress) {
        MouseHeld = true;
    }
    else if (type == AspMouseEventRelease) {
        MouseHeld = false;
    }
    
    wm->onMouseButtonEvent(makePoint(x, y), button, type);
}

void GlfwInputSource::onKeyEvent(unsigned int charCode)
{
    std::shared_ptr<WindowManager> wm = Server::getSingleton()->getWindowManager().lock();
    wm->onKeyEvent(charCode);
}

void GlfwInputSource::ErrorCallback(int error, const char* description)
{
}

void GlfwInputSource::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void GlfwInputSource::CharCallback(GLFWwindow* window, unsigned int codepoint)
{
    GlfwInputSource* is = static_cast<GlfwInputSource*>(glfwGetWindowUserPointer(window));
    is->onKeyEvent(codepoint);
}

void GlfwInputSource::CursorCallback(GLFWwindow* window, double x, double y)
{
    MousePosX = x;
    MousePosY = y;
    
    GlfwInputSource* is = static_cast<GlfwInputSource*>(glfwGetWindowUserPointer(window));
    is->onMouseMoveEvent(MousePosX, MousePosY);
}

void GlfwInputSource::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GlfwInputSource* is = static_cast<GlfwInputSource*>(glfwGetWindowUserPointer(window));
    int aspMouseButtonEventType = 0;
    int aspMouseButton = 0;
    
    switch (action) {
        case GLFW_PRESS:
            aspMouseButtonEventType = AspMouseEventPress;
            break;
            
        case GLFW_RELEASE:
            aspMouseButtonEventType = AspMouseEventRelease;
            break;
    }
    
    switch (button) {
        case GLFW_MOUSE_BUTTON_RIGHT:
            aspMouseButton = AspMouseButtonRight;
            break;
            
        case GLFW_MOUSE_BUTTON_LEFT:
            aspMouseButton = AspMouseButtonLeft;
            break;
            
        case GLFW_MOUSE_BUTTON_MIDDLE:
            aspMouseButton = AspMouseButtonMiddle;
            break;
    }
    
    is->onMouseButtonEvent(MousePosX, MousePosY, aspMouseButton, aspMouseButtonEventType);
}

