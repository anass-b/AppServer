#include <nativecode/application.h>
#include <private/server.h>
#include <pthread.h>
#include <signal.h>

namespace app
{
    Application* Application::_sharedInstance = nullptr;
    
    Application::Application()
    {
        _server = new Server;
        _delegate = nullptr;
        _threadInitialized = false;
    }
    
    Application* Application::getSingleton()
    {
        if (!_sharedInstance) {
            _sharedInstance = new Application();
        }
        
        return _sharedInstance;
    }
    
    void Application::addWindow(Window *w)
    {
        _windows.push_back(w);
    }
    
    void Application::setDelegate(ApplicationDelegate *d)
    {
        _delegate = d;
    }
    
    std::vector<Window*> Application::getWindows()
    {
        return _windows;
    }
    
    void Application::avoidBusyWait(const long nsec)
    {   
        timespec tim;
        tim.tv_sec  = 0;
        tim.tv_nsec = nsec;
        nanosleep(&tim, NULL);
    }
    
    void* Application::viewUpdateWatcher(void *ptr)
    {
        Application *app = Application::getSingleton();
        
        while (app->_runLoop) {
            
            Application::avoidBusyWait();
            
            for (int i = 0; i < app->_windows.size(); i++) {
                Window *window = app->_windows.at(i);
                if (window->getRootView()->needsRedraw()) {
                    window->flush();
                }
            }
        }
        
        return nullptr;
    }
    
    int Application::run(ApplicationDelegate *delegate)
    {
        Application *app = Application::getSingleton();
        
        if (delegate) {
            delegate->applicationStarted();
        }
        
        pthread_t thread;
        if (pthread_create(&thread, NULL, Application::viewUpdateWatcher, NULL)) {
            throw std::runtime_error(strerror(errno));
        }
        
        while (_runLoop) {
            InputEvent *evt = _server->waitInputEvent();
            
            if (!evt) continue;
            
            for (int i = 0; i < app->_windows.size(); i++) {
                Window *targetWin = app->_windows.at(i);
                if (targetWin->getId() == evt->winId) {
                    targetWin->dispatchInputEvent(evt);
                }
            }
        }
        
        /*while (true) {
            std::cout << "I'm here :)" << std::endl;
            sleep(1);
        }*/
        
        _server->unregisterClient();
        
        return 0;
    }
    
    Server *Application::getServer()
    {
        return _server;
    }
    
    int Application::quit()
    {
        _runLoop = false;        
        return 0;
    }
}

