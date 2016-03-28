#include <iostream>
#include <pthread.h>
#include <asl/asl.h>
#include <protocol.h>
#include <unistd.h>
#include <cairo/cairo.h>

int main(int argc, char *argv[])
{
    void *ctx = aslCreateContext();
    aslSubscribe(ctx);

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    cairo_t* context = cairo_create(surface);
    cairo_set_source_rgb (context, 1.0, 1.0, 0.0);
    cairo_paint (context);

    cairo_surface_flush(surface);
    unsigned char* data = cairo_image_surface_get_data(surface);

    int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, cairo_image_surface_get_width(surface));
    unsigned long dataSize = stride * cairo_image_surface_get_height(surface);

    aslCreateWindow(ctx, data, dataSize, 100, 100, 800, 600, AslWindowRasterARGB);

    AslEvent event;
    while (true) {        
        event = aslWaitEvent(ctx);
        
        if (event.type == AslEventTypeMouseButton) {
            if (event.mouseEvent.button == AslMouseButtonLeft) {
                if (event.mouseEvent.buttonState == AslMouseButtonStatePressed) {
                    std::cout << "Mouse Left Pressed" << std::endl;
                }
                else if (event.mouseEvent.buttonState == AslMouseButtonStateReleased)
                {
                    std::cout << "Mouse Left Released" << std::endl;
                }
            }
            else if (event.mouseEvent.button == AslMouseButtonRight) {
                if (event.mouseEvent.buttonState == AslMouseButtonStatePressed) {
                    std::cout << "Mouse Right Pressed" << std::endl;
                }
                else if (event.mouseEvent.buttonState == AslMouseButtonStateReleased)
                {
                    std::cout << "Mouse Right Released" << std::endl;
                }
            }
        }
        else if (event.type == AslEventTypeMouseMove) {
            std::cout << "Mouse Move" << std::endl;
        }
        else if (event.type == AslEventTypeMouseScroll) {
            std::cout << "Mouse Scroll (" << event.mouseEvent.scrollX << "," << event.mouseEvent.scrollY << ")" << std::endl;
        }
        else if (event.type == AslEventTypeText) {
            printf("%s\n", event.textEvent.text);
        }
        else if (event.type == AslEventTypeKey) {
            if (event.keyEvent.scancode == ASL_SCANCODE_RETURN) {
                std::cout << "Return Key" << std::endl;
            }
            else if (event.keyEvent.scancode == ASL_SCANCODE_BACKSPACE) {
                std::cout << "Backspace Key" << std::endl;
            }
            else {
                std::cout << "Key" << std::endl;
            }
        }
    }

    return 0;
}
