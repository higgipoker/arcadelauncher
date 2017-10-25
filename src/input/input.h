//
//  Input.h
//  sensi2
//
//  Created by Paul Higgins on 29/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#ifndef __sensi2__Input__
#define __sensi2__Input__

#include "SDL.h"
#include "../utils/timer.h"
#include <iostream>

enum input_event {
    INPUT_NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FIRE_DOWN,
    FIRE_UP,
    FIRE_LENGTH,
    SINGLE_TAP,
    DOUBLE_TAP,
    QUIT,
    TOTAL_EVENTS
};

enum input_trigger { TRIGGER_NONE = 0, PRESS, RELEASE, REPEAT };

struct Event {
    Event() {
        evt = INPUT_NONE;
        trigger = TRIGGER_NONE;
    }
    input_event evt;
    input_trigger trigger;
};

//  -------------------------------------------------------
//
//  Input device
//
//  -------------------------------------------------------

class Input {

  public:
    /* constructor */
    Input();

    virtual ~Input();

    /* init the device */
    virtual void init() = 0;

    /* get new status from device */
    virtual Event getInputEvent(SDL_Event event);

    /* polymorphic on update */
    virtual void onUpdate(SDL_Event event) = 0;

    /* de-init */
    virtual void release() = 0;

    //  event states
    int event_states[TOTAL_EVENTS];

    /* debug */
    void printStates() {
        // 		print ( event_states[UP] );
        // 		print ( event_states[DOWN] );
        // 		print ( event_states[LEFT] );
        // 		print ( event_states[RIGHT] );
    }

    virtual void reset();

    virtual void onReset() = 0;

  protected:
    // track for fire button being held
    Timer fire_timer;
    Timer repeat_timer;
    bool key_down;
    bool first_repeat;
    int repeat_speed;
};

#endif /* defined(__sensi2__Input__) */
