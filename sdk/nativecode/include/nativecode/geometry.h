#ifndef _MANGO_GEOMETRY_H
#define _MANGO_GEOMETRY_H

namespace app
{
    typedef struct {
        double x;
        double y;
    } Point;
    
    typedef struct {
        double width;
        double height;
    } Size;
    
    typedef struct {
        Point location;
        Size size;
    } Rect;
    
    Rect makeRect(double x, double y, double w, double h);
    Point makePoint(double x, double y);
    Size makeSize(double w, double h);
}

#endif