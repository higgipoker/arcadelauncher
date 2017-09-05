//
//  Joystick.h
//  sensi2
//
//  Created by Paul Higgins on 28/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#ifndef __sensi2__Joystick__
#define __sensi2__Joystick__

#include "input.h"
#include "SDL.h"
#include <iostream>

//  -------------------------------------------------------
//
//  Joystick//
//  -------------------------------------------------------

class Joystick : public Input {

  public:
    /* constructor */
    Joystick();

    /* destructor */
    virtual ~Joystick();

    /*init */
    virtual void init();

    /* update */
    virtual void onUpdate(SDL_Event event);

    /* release */
    virtual void release();

    virtual void onReset();

  protected:
    // sdl joystick
    SDL_Joystick *stick;

    //  time double tap
    Timer tap_timer;
    int tap;
    bool button_down;
};

#endif /* defined(__sensi2__Joystick__) */
