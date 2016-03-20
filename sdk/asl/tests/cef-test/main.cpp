#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>

class RenderHandler : public CefRenderHandler
{
public:
    RenderHandler()
    {
    }
    
    // CefRenderHandler interface
public:
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) OVERRIDE
    {
        return true;
    }
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) OVERRIDE
    {
    }
    
    // CefBase interface
public:
    IMPLEMENT_REFCOUNTING(RenderHandler);
    
};


// for manual render handler
class BrowserClient : public CefClient
{
public:
    BrowserClient(RenderHandler *renderHandler)
    : m_renderHandler(renderHandler)
    {;}
    
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
        return m_renderHandler;
    }
    
    CefRefPtr<CefRenderHandler> m_renderHandler;
    
    IMPLEMENT_REFCOUNTING(BrowserClient);
};

int main(int argc, char *argv[])
{
    CefMainArgs args(argc, argv);
    
    int result = CefExecuteProcess(args, nullptr, nullptr);
    // checkout CefApp, derive it and set it as second parameter, for more control on
    // command args and resources.
    if (result >= 0) // child proccess has endend, so exit.
    {
        return result;
    }
    if (result == -1)
    {
        // we are here in the father proccess.
    }

    CefSettings settings;
    
    // checkout detailed settings options http://magpcss.org/ceforum/apidocs/projects/%28default%29/_cef_settings_t.html
    // nearly all the settings can be set via args too.
    // settings.multi_threaded_message_loop = true; // not supported, except windows
    // CefString(&settings.browser_subprocess_path).FromASCII("sub_proccess path, by default uses and starts this executeable as child");
    // CefString(&settings.cache_path).FromASCII("");
    // CefString(&settings.log_file).FromASCII("");
    // settings.log_severity = LOGSEVERITY_DEFAULT;
    // CefString(&settings.resources_dir_path).FromASCII("");
    // CefString(&settings.locales_dir_path).FromASCII("");
    
    bool res = CefInitialize(args, settings, nullptr, nullptr);
    // CefInitialize creates a sub-proccess and executes the same executeable, as calling CefInitialize, if not set different in settings.browser_subprocess_path
    // if you create an extra program just for the childproccess you only have to call CefExecuteProcess(...) in it.
    if (!res)
    {
        // handle error
        return -1;
    }
    
    CefShutdown();
    
    return 0;
}
