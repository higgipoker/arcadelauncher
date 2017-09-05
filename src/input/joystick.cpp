//
//  Joystick.cpp
//  sensi2
//
//  Created by Paul Higgins on 28/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#include "joystick.h"

//  --------------------------------------------------
//  constructor
//  --------------------------------------------------
Joystick::Joystick() {
    stick = NULL;
    tap = false;
    button_down = false;
}

//  --------------------------------------------------
//  destructor
//  --------------------------------------------------
Joystick::~Joystick() {}

//  --------------------------------------------------
//  release
//  --------------------------------------------------
void Joystick::release() {
    SDL_JoystickClose(stick);
}

//  --------------------------------------------------
//  init
//  --------------------------------------------------
void Joystick::init() {
    stick = SDL_JoystickOpen(0);

    if(stick == NULL) {
        std::cout << ("PROBLEM OPENING JOYSTICK 0") << std::endl;

    } else {
        std::cout << ("JOYSTICK 0 OPENED SUCCESSFULLY") << std::endl;
    }

    SDL_JoystickEventState(SDL_ENABLE);

    if(stick == NULL) {
        std::cout << ("PROBLEM OPENING JOYSTICK 0") << std::endl;

    } else {
        std::cout << ("JOYSTICK 0 OPENED SUCCESSFULLY") << std::endl;
    }

    SDL_JoystickEventState(SDL_ENABLE);

}

//  --------------------------------------------------
//  update
//  --------------------------------------------------
void Joystick::onUpdate(SDL_Event event) {

    // printStates();

    event_states[FIRE_UP] = 0;
    event_states[DOUBLE_TAP] = 0;
    event_states[SINGLE_TAP] = 0;

    if(tap_timer.get_ticks() > 120) {
        if(tap == 1) {
            event_states[SINGLE_TAP] = 1;
        }

        tap = 0;
    }

    if(event.type == SDL_JOYAXISMOTION) {

        // check left-right movement
        if(event.jaxis.axis == 0) {
            // if joystick moving left
            if(event.jaxis.value < -3200) {
                // move left
                event_states[LEFT] = 1;
            }

            else {
                event_states[LEFT] = 0;
            }

            // if joystick moving right
            if(event.jaxis.value > 3200) {
                // move right
                event_states[RIGHT] = 1;
            }

            else {

                event_states[RIGHT] = 0;
            }
        }

        // check up-down movement
        if(event.jaxis.axis == 1) {
            // if joystick moving up
            if(event.jaxis.value < -3200) {
                // move up
                event_states[UP] = 1;
            }

            else {
                event_states[UP] = 0;
            }

            // if joystick moving down
            if(event.jaxis.value > 3200) {
                // move down
                event_states[DOWN] = 1;
            }

            else {
                event_states[DOWN] = 0;
            }
        }
    }

    if(event.type == SDL_JOYHATMOTION) {
        if(event.jhat.value & SDL_HAT_UP) {
            event_states[UP] = 1;

        } else {
            event_states[UP] = 0;
        }

        if(event.jhat.value & SDL_HAT_LEFT) {
            event_states[LEFT] = 1;

        } else {
            event_states[LEFT] = 0;
        }

        if(event.jhat.value & SDL_HAT_DOWN) {
            event_states[DOWN] = 1;

        } else {
            event_states[ UP ] = 0;
        }

        if(event.jhat.value & SDL_HAT_RIGHT) {
            event_states[RIGHT] = 1;

        } else {
            event_states[RIGHT] = 0;
        }
    }

    if(event.type == SDL_JOYBUTTONDOWN) {  /* Handle Joystick Button Presses */

        if(button_down)
            return;

        button_down = true;

        // if ( event.jbutton.button == 1 ) {
        if(true) {  // any button
            event_states[DOUBLE_TAP] = 0;

            if(event_states[FIRE_DOWN] == 0) {

                event_states[FIRE_DOWN] = 1;

                fire_timer.start();
            }
        }
    }

    if(event.type == SDL_JOYBUTTONUP) {  /* Handle Joystick Button Presses */
        // if ( event.jbutton.button == 1 ) {

        if(!button_down)
            return;

        button_down = false;

        if(true) {  // any button
            event_states[DOUBLE_TAP] = 0;

            if(event_states[FIRE_DOWN] == 1) {
                event_states[FIRE_DOWN] = 0;

                // if( fire_timer.get_ticks() > 90 ){
                if(true) {

                    event_states[FIRE_UP] = 1;
                    event_states[FIRE_LENGTH] = fire_timer.get_ticks();
                    fire_timer.stop();

                } else {

                    tap++;
                    tap_timer.start();

                    if(tap >= 2) {

                        tap = 0;
                        event_states[DOUBLE_TAP] = 1;

                    } else {
                        // tap = 0;
                    }

                    button_down = false;

                    if(true) {  // any button
                        event_states[DOUBLE_TAP] = 0;

                        if(event_states[FIRE_DOWN] == 1) {
                            event_states[FIRE_DOWN] = 0;


                            //if( fire_timer.get_ticks() > 90 ){
                            if(true) {

                                event_states[FIRE_UP] = 1;
                                event_states[FIRE_LENGTH] = fire_timer.get_ticks();
                                fire_timer.stop();

                            } else {

                                tap++;
                                tap_timer.start();


                                if(tap >= 2) {

                                    tap = 0;
                                    event_states[DOUBLE_TAP] = 1;

                                } else {
                                    //tap = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void Joystick::onReset() {
    tap = 0;
    tap_timer.stop();
    button_down = false;
}



