#include <iostream>
#include <pthread.h>
#include <Asl/CApi.h>
#include <protocol.h>
#include <unistd.h>
#include <cairo/cairo.h>

int main(int argc, char *argv[])
{
    aslInit();
    aslSubscribe();

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    cairo_t* context = cairo_create(surface);
    cairo_set_source_rgb (context, 1.0, 1.0, 0.0);
    cairo_paint (context);

    cairo_surface_flush(surface);
    unsigned char* data = cairo_image_surface_get_data(surface);

    int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, cairo_image_surface_get_width(surface));
    unsigned long dataSize = stride * cairo_image_surface_get_height(surface);

    unsigned long windowId = aslNewWindow(data, dataSize, 100, 100, 800, 600, AspWindowRasterARGB, true);

    AslEvent event;
    while (true) {        
        event = aslWaitEvent();
        
        if (event.type == AspEventMouseInput) {       
            if (event.inputEvent.mouseEvent.type == AspMouseEventPress) {
                std::cout << "Mouse Press" << std::endl;
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventRelease) {
                std::cout << "Mouse Release" << std::endl;
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventMove) {
                std::cout << "Mouse Move" << std::endl;
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventDrag) {
                std::cout << "Mouse Drag" << std::endl;
            }
        }
    }

    return 0;
}
