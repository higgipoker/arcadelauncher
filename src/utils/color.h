//
//  Color.h
//  sensi2
//
//  Created by Paul Higgins on 19/07/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#pragma once

#include <iostream>

class Color {
  public:
    Color() { r = g = b = a = 0; }

    Color(int _r, int _g, int _b, int _a = 255) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    int r, g, b, a;

  private:
};
