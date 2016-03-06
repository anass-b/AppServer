//
//  Geometry.h
//  appserver
//
//  Created by Anass on 13/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef appserver_Geometry_h
#define appserver_Geometry_h

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
    
    Rect makeRect(double x, double y, double w, double h);
    Point makePoint(double x, double y);
    Size makeSize(double w, double h);
}

#endif
