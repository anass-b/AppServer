//
//  GlfwInputSource.h
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_GlfwInputSource_h
#define appserver_GlfwInputSource_h

#include <memory>
#include <GLFW/glfw3.h>
#include <InputSource.h>

namespace appserver
{
    class GlfwInputSource : public InputSource
    {
    public:
        GlfwInputSource(std::shared_ptr<Workspace> screen);
        virtual void pollEvents();
        virtual void onMouseMoveEvent(double x, double y);
        virtual void onMouseButtonEvent(double x, double y, int button, int type);
        virtual void onKeyEvent(unsigned int charCode);
    private:
        static void ErrorCallback(int error, const char* description);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CharCallback(GLFWwindow* window, unsigned int codepoint);
        static void CursorCallback(GLFWwindow* window, double x, double y);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    private:
        static double MousePosX;
        static double MousePosY;
        static bool MouseHeld;
    };
}

#endif
