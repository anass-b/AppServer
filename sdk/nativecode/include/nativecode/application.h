#ifndef _MANGO_APPLICATION_H
#define _MANGO_APPLICATION_H

#include <iostream>
#include <vector>
#include <nativecode/window.h>

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

namespace app
{
    const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;
    
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
        static void avoidBusyWait(const long nsec = INTERVAL_MS);
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