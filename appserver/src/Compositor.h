//
//  Compositor.h
//  appserver
//
//  Created by Anass on 10/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <iostream>
#include <memory>
#include <vector>

#include <Geometry.h>
#include <Window.h>

namespace appserver
{
    class Compositor
    {
    public:
        Compositor();
        virtual ~Compositor();
        virtual void compose() = 0;
        void setSize(Size size);
        Size getSize() const;
        void addWindow(std::shared_ptr<Window> w);
        void removeWindow(TWindowId id);
        void removeWindows(unsigned long int appId);
        std::shared_ptr<Window> findWindow(TWindowId id);
        std::shared_ptr<Window> findWindowInLocation(Point location);
        std::vector<std::shared_ptr<Window>>::iterator findWindowPosition(TWindowId id);
        void sortWindowsByZOrder();
        void bringWindowToFront(std::shared_ptr<Window> w);
        std::shared_ptr<Window> getTopMostWindow();
        std::vector<std::shared_ptr<Window>>& getWindows() const;
        TWindowZ computeMaxZ() const;
        TWindowZ getNewWindowZ() const;
        
    protected:
        static bool compareWindowsZDsc(std::shared_ptr<Window> winA, std::shared_ptr<Window> winB);
        
    protected:
        Size _size;
        mutable std::vector<std::shared_ptr<Window>> _windows;
    };
}

#endif
