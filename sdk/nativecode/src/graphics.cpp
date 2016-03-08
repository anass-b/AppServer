#include <nativecode/graphics.h>
#include <cairo/cairo.h>

#define DK_CAIRO_CONTEXT(s) (cairo_t*)s
#define DK_CAIRO_SURFACE(s) (cairo_surface_t*)s

namespace app
{
    Surface::Surface(Size size)
    {
        _size = size;
        _surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, size.width, size.height);
        _context = cairo_create(DK_CAIRO_SURFACE(_surface));
    }
    
    Surface::Surface(const char *filename)
    {
        _surface = cairo_image_surface_create_from_png(filename);
        _size.width = cairo_image_surface_get_width(DK_CAIRO_SURFACE(_surface));
        _size.height = cairo_image_surface_get_height(DK_CAIRO_SURFACE(_surface));
        _context = cairo_create(DK_CAIRO_SURFACE(_surface));
    }
    
    void Surface::blit(Surface *surface, Rect src, Point dst)
    {
        cairo_t *cr = cairo_create(DK_CAIRO_SURFACE(_surface));
        
        cairo_set_source_surface (cr, DK_CAIRO_SURFACE(surface->_surface), dst.x - src.location.x, dst.y - src.location.y);
        cairo_rectangle (cr, dst.x, dst.y, src.size.width, src.size.height);
        cairo_fill (cr);
        
        cairo_destroy(cr);
    }
    
    void Surface::fill(Color color)
    {
        ColorF colorF = colorToColorF(color);
        cairo_set_source_rgb (DK_CAIRO_CONTEXT(_context), colorF.r, colorF.g, colorF.b);
        cairo_paint (DK_CAIRO_CONTEXT(_context));
    }
    
    ColorF Surface::colorToColorF(Color color)
    {
        ColorF cf;
        cf.r = color.r / 255.0f;
        cf.g = color.g / 255.0f;
        cf.b = color.b / 255.0f;
        cf.a = color.a / 255.0f;
        
        return cf;
    }
    
    Surface::~Surface()
    {
        cairo_surface_destroy(DK_CAIRO_SURFACE(_surface));
        cairo_destroy(DK_CAIRO_CONTEXT(_context));
    }
    
    unsigned char* Surface::getData()
    {
        cairo_surface_flush(DK_CAIRO_SURFACE(_surface));
        return cairo_image_surface_get_data(DK_CAIRO_SURFACE(_surface));
    }
    
    unsigned long Surface::getDataSize()
    {
        int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, _size.width);
        return stride * _size.height;
    }
    
    Size Surface::getSize()
    {
        return _size;
    }
    
    Color makeColor(unsigned int r, unsigned int g, unsigned int b, unsigned a)
    {
        Color c;
        c.r = r;
        c.g = g;
        c.b = b;
        c.a = a;
        
        return c;
    }
    
    Color makeColor(unsigned int r, unsigned int g, unsigned int b)
    {
        Color c;
        c.r = r;
        c.g = g;
        c.b = b;
        c.a = 0;
        
        return c;
    }
}