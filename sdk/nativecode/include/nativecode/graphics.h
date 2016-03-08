#ifndef _MANGO_GRAPHICS_H
#define _MANGO_GRAPHICS_H

#include <nativecode/geometry.h>

namespace app
{
    struct Color
    {
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;
    };
    
    struct ColorF
    {
        double r;
        double g;
        double b;
        double a;
    };
    
    Color makeColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    Color makeColor(unsigned int r, unsigned int g, unsigned int b);
    
    class Surface
    {
    public:
        Surface(Size size);
        Surface(const char *filename);
        void blit(Surface *surface, Rect src, Point dst);
        void fill(Color color);
        Size getSize();
        unsigned char* getData();
        unsigned long getDataSize();
        ~Surface();
    private:
        ColorF colorToColorF(Color color);
    private:
        Size _size;
        void *_surface;
        void *_context;
    };
}

#endif