//
//  Input.cpp
//  sensi2
//
//  Created by Paul Higgins on 29/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#include "input.h"

#define INITIAL_REPEAT_SPEED 500
#define MIN_REPEAT_SPEED 100
#define REPEATE_ACCELERATION 5

//  --------------------------------------------------
//  constructor
//  --------------------------------------------------
Input::Input() {
    event_states[UP] = 0;
    event_states[DOWN] = 0;
    event_states[LEFT] = 0;
    event_states[RIGHT] = 0;
    event_states[FIRE_DOWN] = 0;
    event_states[FIRE_UP] = 0;
    event_states[FIRE_LENGTH] = 0;
    event_states[SINGLE_TAP] = 0;
    event_states[DOUBLE_TAP] = 0;

    key_down = false;
    first_repeat = true;
    repeat_speed = INITIAL_REPEAT_SPEED;
}

Input::~Input() {}

Event Input::getInputEvent(SDL_Event event) {
    Event evt;

    // set the states for specific input device
    onUpdate(event);

    // work out which event to return
    switch (event.type) {

    case SDL_KEYDOWN:

        if (!key_down) {
            key_down = true;
            repeat_timer.start();
            first_repeat = true;
            repeat_speed = INITIAL_REPEAT_SPEED;

        } else {
            first_repeat = false;
            repeat_speed -= REPEATE_ACCELERATION;

            if (repeat_speed < MIN_REPEAT_SPEED) {
                repeat_speed = MIN_REPEAT_SPEED;
            }
        }

        switch (event.key.keysym.sym) {

        case SDLK_ESCAPE:
            evt.evt = QUIT;
            break;
        }

        break;

    case SDL_KEYUP:

        key_down = false;

        break;

    case SDL_TEXTEDITING:
        break;

    case SDL_TEXTINPUT:
        break;

    default:
        break;
    };

    // FIRE BUTTON ALWAYS ON BUTTON UP
    if (event_states[FIRE_UP]) {
        evt.evt = SINGLE_TAP;
    }

    // RIGHT EVENT
    if (event_states[RIGHT]) {
        if (first_repeat || repeat_timer.get_ticks() > repeat_speed) {
            evt.evt = RIGHT;
            repeat_timer.start();
            first_repeat = false;
        }
    }

    // LEFT EVENT
    if (event_states[LEFT]) {
        if (first_repeat || repeat_timer.get_ticks() > repeat_speed) {
            evt.evt = LEFT;
            repeat_timer.start();
            first_repeat = false;
        }
    }

    // UP EVENT
    if (event_states[UP]) {
        if (first_repeat || repeat_timer.get_ticks() > repeat_speed) {
            evt.evt = UP;
            repeat_timer.start();
            first_repeat = false;
        }
    }

    // DOWN EVENT
    if (event_states[DOWN]) {
        if (first_repeat || repeat_timer.get_ticks() > repeat_speed) {
            evt.evt = DOWN;
            repeat_timer.start();
            first_repeat = false;
        }
    }

    if (event.type == SDL_QUIT) {
        evt.evt = QUIT;
    }

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDLK_1] && keystate[SDLK_2]) {
        evt.evt = QUIT;
    }
    return evt;
}

void Input::reset() {
    for (unsigned int i = 0; i < TOTAL_EVENTS; i++) {
        event_states[i] = 0;
    }

    onReset();
}
