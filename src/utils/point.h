//
//  Point.h
//  sensi2
//
//  Created by Paul Higgins on 19/07/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#pragma once
#include <SDL.h>
#include <iostream>

class Point : public SDL_Point {
  public:
    Point(int _x, int _y) {
        x = _x;
        y = _y;
    }

    Point() { x = y = 0; }

};
