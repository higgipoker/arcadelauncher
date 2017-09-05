//
//  Keyboard.cpp
//  sensi2
//
//  Created by Paul Higgins on 29/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#include "keyboard.h"

//  --------------------------------------------------
//  init
//  --------------------------------------------------
void Keyboard::init() {}

//  --------------------------------------------------
//  update
//  --------------------------------------------------
void Keyboard::onUpdate(SDL_Event event) {

    event_states[FIRE_UP] = 0;

    if (event.type == SDL_KEYDOWN) {

        // Adjust the velocity
        switch (event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_r:
            event_states[UP] = 1;
            break;
        case SDLK_DOWN:
        case SDLK_f:
            event_states[DOWN] = 1;
            break;
        case SDLK_LEFT:
        case SDLK_d:
            event_states[LEFT] = 1;
            break;
        case SDLK_RIGHT:
        case SDLK_g:
            event_states[RIGHT] = 1;
            break;
        case SDLK_RETURN:
        case SDLK_RETURN2:
        case SDLK_LCTRL:
        case SDLK_LALT:
        case SDLK_SPACE:
        case SDLK_LSHIFT:
        case SDLK_z:
        case SDLK_x:

        case SDLK_a:
        case SDLK_s:
        case SDLK_q:
        case SDLK_w:
        case SDLK_i:
        case SDLK_k:

            event_states[FIRE_DOWN] = 1;
            if (release_flag) {
                release_flag = false;
                fire_timer.start();
            }

            break;
        }
    }
    // If a key was released
    else if (event.type == SDL_KEYUP) {

        switch (event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_r:
            event_states[UP] = 0;
            break;
        case SDLK_DOWN:
        case SDLK_f:
            event_states[DOWN] = 0;
            break;
        case SDLK_LEFT:
        case SDLK_d:
            event_states[LEFT] = 0;
            break;
        case SDLK_RIGHT:
        case SDLK_g:
            event_states[RIGHT] = 0;
            break;

        case SDLK_RETURN:
        case SDLK_RETURN2:
        case SDLK_LCTRL:
        case SDLK_LALT:
        case SDLK_SPACE:
        case SDLK_LSHIFT:
        case SDLK_z:
        case SDLK_x:

        case SDLK_a:
        case SDLK_s:
        case SDLK_q:
        case SDLK_w:
        case SDLK_i:
        case SDLK_k:

            if (event_states[FIRE_DOWN] == 1) {
                event_states[FIRE_DOWN] = 0;
                event_states[FIRE_UP] = 1;
                event_states[FIRE_LENGTH] = fire_timer.get_ticks();
                fire_timer.stop();
                release_flag = true;
            }
            break;
        }
    }

    // printStates();
}

//  --------------------------------------------------
//  release
//  --------------------------------------------------
void Keyboard::release() {}
