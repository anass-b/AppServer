#include <iostream>
#include <thread>
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include <asl/asl.h>
#include <cairo.h>

void* gCtx = nullptr;
uint32_t gWindowId = 0;
const int kWindowWidth = 1280;
const int kWindowHeight = 700;

const int kBrowserWidth = kWindowWidth;
const int kBrowserHeight = kWindowHeight;

class RenderHandler : public CefRenderHandler {
public:
    RenderHandler()
    {
    }

public:
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE
    {
        rect = CefRect(0, 0, kBrowserWidth, kBrowserHeight);
        return true;
    }
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE
    {
        const bool kUseCompression = true;
        if (gWindowId == 0) {
            gWindowId = aslCreateWindow(gCtx, (unsigned char*)buffer, width * height * 4, 0, 0, kWindowWidth, kWindowHeight, AslWindowRasterARGB);
        }
        else {
            if (dirtyRects.size() > 1) {
                aslUpdateWindow(gCtx, gWindowId, (unsigned char*)buffer, width * height * 4, 0, 0, width, height, kUseCompression);
            }
            else if (dirtyRects.size() == 1) {
                CefRect dirtyRect = dirtyRects.at(0);

                if (dirtyRect.x == 0 && dirtyRect.y == 0 && dirtyRect.width == width && dirtyRect.height == height) {
                    aslUpdateWindow(gCtx, gWindowId, (unsigned char*)buffer, width * height * 4, 0, 0, width, height, kUseCompression);
                }
                else {
                    int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
                    cairo_surface_t* src = cairo_image_surface_create_for_data((unsigned char*)buffer, CAIRO_FORMAT_ARGB32, width, height, stride);

                    cairo_surface_t* dst = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, dirtyRect.width, dirtyRect.height);
                    cairo_t* cr = cairo_create(dst);

                    int source_x = dirtyRect.x;
                    int source_y = dirtyRect.y;
                    int dest_x = 0;
                    int dest_y = 0;
                    cairo_set_source_surface(cr, src, dest_x - source_x, dest_y - source_y);
                    cairo_rectangle(cr, dest_x, dest_y, dirtyRect.width, dirtyRect.height);
                    cairo_fill(cr);

                    unsigned char* data = cairo_image_surface_get_data(dst);

                    aslUpdateWindow(gCtx, gWindowId, data, dirtyRect.width * dirtyRect.height * 4, dirtyRect.x, dirtyRect.y, dirtyRect.width, dirtyRect.height, kUseCompression);

                    cairo_destroy(cr);
                    cairo_surface_destroy(src);
                    cairo_surface_destroy(dst);
                }
            }
        }
    }

public:
    IMPLEMENT_REFCOUNTING(RenderHandler)
};

class BrowserClient : public CefClient {
public:
    BrowserClient(RenderHandler* renderHandler)
        : m_renderHandler(renderHandler)
    {
    }

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE
    {
        return m_renderHandler;
    }

    CefRefPtr<CefRenderHandler> m_renderHandler;

    IMPLEMENT_REFCOUNTING(BrowserClient)
};

int scancodeToNativeKeycode(int32_t scancode)
{
    switch (scancode) {
    case ASL_SCANCODE_BACKSPACE:
        return 8;
    case ASL_SCANCODE_RETURN:
        return 13;
    case ASL_SCANCODE_TAB:
        return 9;
    case ASL_SCANCODE_DELETE:
        return 46;
    case ASL_SCANCODE_INSERT:
        return 45;
    case ASL_SCANCODE_END:
        return 35;
    case ASL_SCANCODE_HOME:
        return 36;
    case ASL_SCANCODE_UP:
        return 38;
    case ASL_SCANCODE_DOWN:
        return 40;
    case ASL_SCANCODE_LEFT:
        return 37;
    case ASL_SCANCODE_RIGHT:
        return 39;
    }

    return -1;
}

void* eventThread(void* ptr)
{
    CefBrowser* browser = (CefBrowser*)ptr;
    AslEvent event;
    bool mouseDown = false;
    while (true) {
        event = aslWaitEvent(gCtx);

        if (event.type == AslEventTypeMouseButton && event.mouseEvent.button == AslMouseButtonLeft && event.mouseEvent.buttonState == AslMouseButtonStatePressed) {
            mouseDown = true;
            CefMouseEvent mouseEvent;
            mouseEvent.x = event.mouseEvent.x;
            mouseEvent.y = event.mouseEvent.y;
            browser->GetHost()->SendMouseClickEvent(mouseEvent, MBT_LEFT, false, 1);
        }
        else if (event.type == AslEventTypeMouseButton && event.mouseEvent.button == AslMouseButtonLeft && event.mouseEvent.buttonState == AslMouseButtonStateReleased) {
            mouseDown = false;
            CefMouseEvent mouseEvent;
            mouseEvent.x = event.mouseEvent.x;
            mouseEvent.y = event.mouseEvent.y;
            browser->GetHost()->SendMouseClickEvent(mouseEvent, MBT_LEFT, true, 1);
        }
        else if (event.type == AslEventTypeMouseMove) {
            CefMouseEvent mouseEvent;
            if (mouseDown) {
                mouseEvent.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;
            }
            mouseEvent.x = event.mouseEvent.x;
            mouseEvent.y = event.mouseEvent.y;
            browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);
        }
        else if (event.type == AslEventTypeMouseScroll) {
            CefMouseEvent mouseEvent;
            mouseEvent.x = event.mouseEvent.x;
            mouseEvent.y = event.mouseEvent.y;
            int velocity = 20;
            browser->GetHost()->SendMouseWheelEvent(mouseEvent, event.mouseEvent.scrollX * velocity, event.mouseEvent.scrollY * velocity);
        }
        else if (event.type == AslEventTypeText) {
            //printf("%s %zu\n", event.textEvent.text, event.textEvent.textSize);
            for (size_t i = 0; i < event.textEvent.textSize; i++) {
                CefKeyEvent cefEvent;
                cefEvent.type = KEYEVENT_CHAR;
                char16 c = event.textEvent.text[0];
                cefEvent.character = c;
                browser->GetHost()->SendKeyEvent(cefEvent);
            }
        }
        else if (event.type == AslEventTypeKey) {
            CefKeyEvent cefEvent;
            cefEvent.is_system_key = false;
            cefEvent.modifiers = 0;

            if (event.keyEvent.state == AslKeyStatePressed) {
                cefEvent.type = KEYEVENT_RAWKEYDOWN;
            }
            else if (event.keyEvent.state == AslKeyStateReleased) {
                cefEvent.type = KEYEVENT_KEYUP;
            }

            int native_key_code = scancodeToNativeKeycode(event.keyEvent.scancode);
            if (native_key_code != -1) {
                cefEvent.native_key_code = native_key_code;
                cefEvent.windows_key_code = native_key_code;
                cefEvent.character = cefEvent.unmodified_character = native_key_code;
                browser->GetHost()->SendKeyEvent(cefEvent);
                //std::cout << "Key " << native_key_code << std::endl;
            }
        }
    }
    return nullptr;
}

int main(int argc, char* argv[])
{
    std::string url;

    CefMainArgs args(argc, argv);

    int result = CefExecuteProcess(args, nullptr, nullptr);
    if (result >= 0) {
        // the child proccess terminated, we exit.
        return result;
    }
    if (result == -1) {
        // Parent proccess.
        gCtx = aslCreateContext();
        aslSubscribe(gCtx);

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
    if (!res) {
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
    browser->GetHost()->SendFocusEvent(true);

    std::thread thread(eventThread, browser.get());

    CefRunMessageLoop();

    CefShutdown();

    thread.join();

    return 0;
}
