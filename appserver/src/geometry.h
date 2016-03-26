//
//  Geometry.h
//  appserver
//
//  Created by Anass on 13/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace appserver
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

inline Rect makeRect(double x, double y, double w, double h)
{
    Rect rect;
    rect.location.x = x;
    rect.location.y = y;
    rect.size.width = w;
    rect.size.height = h;
    
    return rect;
}

inline Point makePoint(double x, double y)
{
    Point point;
    point.x = x;
    point.y = y;
    
    return point;
}

inline Size makeSize(double w, double h)
{
    Size size;
    size.width = w;
    size.height = h;
    
    return size;
}

}

#endif
