//
//  Geometry.cpp
//  appserver
//
//  Created by Anass on 13/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <Geometry.h>

namespace appserver
{
    Rect makeRect(double x, double y, double w, double h)
    {
        Rect rect;
        rect.location.x = x;
        rect.location.y = y;
        rect.size.width = w;
        rect.size.height = h;
        
        return rect;
    }
    
    Point makePoint(double x, double y)
    {
        Point point;
        point.x = x;
        point.y = y;
        
        return point;
    }
    
    Size makeSize(double w, double h)
    {
        Size size;
        size.width = w;
        size.height = h;
        
        return size;
    }
}
