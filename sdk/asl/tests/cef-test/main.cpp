#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <Asl/CApi.h>
#include <iostream>
#include <pthread.h>
#include <protocol.h>

long gWindowId = -1;
const int kWindowWidth = 1280;
const int kWIndowHeight = 800;

class RenderHandler : public CefRenderHandler
{
public:
    RenderHandler()
    {
    }
    
public:
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) OVERRIDE
    {
        rect = CefRect(0, 0, kWindowWidth, kWIndowHeight);
        return true;
    }
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) OVERRIDE
    {
        if (gWindowId == -1) {
            gWindowId = aslNewWindow((unsigned char*)buffer, width*height*4, 100, 100, kWindowWidth, kWIndowHeight, AspWindowRasterARGB, true);
        }
        else {
            aslUpdateWindowSurface(gWindowId, (unsigned char*)buffer, width*height*4, 100, 100, kWindowWidth, kWIndowHeight);
        }
    }
    
public:
    IMPLEMENT_REFCOUNTING(RenderHandler);
    
};

class BrowserClient : public CefClient
{
public:
    BrowserClient(RenderHandler *renderHandler) : m_renderHandler(renderHandler)
    {      
    }
    
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
        return m_renderHandler;
    }
    
    CefRefPtr<CefRenderHandler> m_renderHandler;
    
    IMPLEMENT_REFCOUNTING(BrowserClient);
};

void* eventThread(void *ptr)
{
    CefBrowser *browser = (CefBrowser*)ptr;       
    AslEvent event;
    while (true) {        
        event = aslWaitEvent();
        
        if (event.type == AspEventMouseInput) {       
            if (event.inputEvent.mouseEvent.type == AspMouseEventPress) {
                CefMouseEvent mouseEvent;
                mouseEvent.x = event.inputEvent.mouseEvent.x;
                mouseEvent.y = event.inputEvent.mouseEvent.y;
                browser->GetHost()->SendMouseClickEvent(mouseEvent, MBT_LEFT, false, 1);
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventRelease) {
                CefMouseEvent mouseEvent;
                mouseEvent.x = event.inputEvent.mouseEvent.x;
                mouseEvent.y = event.inputEvent.mouseEvent.y;
                browser->GetHost()->SendMouseClickEvent(mouseEvent, MBT_LEFT, true, 1);
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventMove) {
                CefMouseEvent mouseEvent;
                mouseEvent.x = event.inputEvent.mouseEvent.x;
                mouseEvent.y = event.inputEvent.mouseEvent.y;
                browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);
            }
            
        }
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    std::string url;
    
    CefMainArgs args(argc, argv);
    
    int result = CefExecuteProcess(args, nullptr, nullptr);
    if (result >= 0)
    {
        // the child proccess terminated, we exit.
        return result;
    }
    if (result == -1)
    {
        // Parent proccess.
        aslInit();
        aslSubscribe();
    
        if (argc == 2) {
            url = argv[1];
        }
        else {
            url = "http://www.google.com";
        }
    }

    CefSettings settings;
    settings.windowless_rendering_enabled = true;
    
    bool res = CefInitialize(args, settings, nullptr, nullptr);
    if (!res)
    {
        return -1;
    }
    
    RenderHandler* renderHandler = new RenderHandler(); 
    
    CefWindowInfo window_info;
#ifdef __APPLE__
    window_info.SetAsWindowless(nullptr, false);
#else
    window_info.SetAsWindowless(0, false);
#endif
    
    CefBrowserSettings browserSettings;
    CefRefPtr<BrowserClient> browserClient = new BrowserClient(renderHandler);    
    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), url, browserSettings, nullptr);
    
    pthread_t thread;
    if (pthread_create(&thread, NULL, eventThread, browser.get())) {
        exit(1);
    }
    
    CefRunMessageLoop();
    
    CefShutdown();
    
    return 0;
}
