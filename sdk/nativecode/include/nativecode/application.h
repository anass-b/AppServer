#ifndef _MANGO_APPLICATION_H
#define _MANGO_APPLICATION_H

#include <iostream>
#include <vector>
#include <nativecode/window.h>

namespace app
{
    class Server;
    
    class ApplicationDelegate
    {
    public:
        virtual void applicationStarted() = 0;
    };
    
    class Application
    {
    private:
        Application();
    public:
        static Application* getSingleton();
        int run(ApplicationDelegate *delegate);
        int quit();
        void addWindow(Window *w);
        void setDelegate(ApplicationDelegate *d);
        std::vector<Window*> getWindows();
        Server *getServer();
    private:
        void avoidBusyWait();
        static void* viewUpdateWatcher(void *ptr);
    public:
        bool _runLoop = true;
        static Application *_sharedInstance;
        std::vector<Window*> _windows;
        ApplicationDelegate *_delegate;
        Server *_server;
        pthread_t _thread;
        bool _threadInitialized;
    };
}

#endif