//
//  Keyboard.h
//  sensi2
//
//  Created by Paul Higgins on 29/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#ifndef __sensi2__Keyboard__
#define __sensi2__Keyboard__

#include "input.h"
#include "../utils/timer.h"
#include <iostream>

//  -------------------------------------------------------
//
//  Keyboard
//
//  -------------------------------------------------------
class Keyboard : public Input {
  public:
    Keyboard() { release_flag = true; }

    /* init */
    virtual void init();

    /* update */
    virtual void onUpdate(SDL_Event event);

    /* release */
    virtual void release();

    virtual void onReset(){/* DO NOTHING */};

  private:
    bool release_flag;
};

#endif /* defined(__sensi2__Keyboard__) */
