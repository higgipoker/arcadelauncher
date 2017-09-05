#include "expand.h"
#define SCALE_SPEED 10
#define END_DELAY 00
// --------------------------------------------------
// Expand
// --------------------------------------------------
Expand::Expand() {
    running = false;
    step = 0;
    delta = 0;
    blocking = true;
    scale_x_step = scale_y_step = offset_x = offset_y = 0;
    end_pending = false;
}

// --------------------------------------------------
// Expand
// --------------------------------------------------
Expand::Expand(Widget *target) : Animation(target) {
    running = false;
    step = 0;
    delta = 0;
    blocking = true;
    scale_x_step = scale_y_step = offset_x = offset_y = 0;
    end_pending = false;

    start();
}

// --------------------------------------------------
// init
// --------------------------------------------------
void Expand::init(SDL_Rect initial_size, SDL_Rect target_size, int step_size) {
    reset = initial_size;
    initial = initial_size;
    target = target_size;
    step = step_size;
    delta = 0;
    offset_x = (target_size.w / 2) - initial_size.x + (initial_size.w / 2);
    offset_y = (target_size.h / 2) - initial_size.y + (initial_size.h / 2);
}

// --------------------------------------------------
// update
// --------------------------------------------------
void Expand::update() {
    if (end_pending) {
        if (end_delay.get_ticks() > END_DELAY) {
            stop();
        }
        return;
    }
    if (timer.get_ticks() > SCALE_SPEED) {

        delta += step;
        initial.w += delta;
        initial.h += delta;

        int new_x = ((target.w / 2) - initial.w / 2);
        int new_y = ((target.h / 2) - initial.h / 2);

        widget->scale(initial.w, initial.h);
        widget->setPosition(new_x, new_y);

        timer.start();

        if (initial.w >= target.w && initial.h >= target.h) {

            //                         widget->setPosition(target.x, target.y);
            //                         widget->scale(target.w, target.h);

            end_delay.start();
            end_pending = true;
        }
    }
}

// --------------------------------------------------
// onStart
// --------------------------------------------------
void Expand::onStart() {
    running = true;
    timer.start();
}

// --------------------------------------------------
// onEnd
// --------------------------------------------------
void Expand::onEnd() {
    end_pending = false;
    timer.stop();
    running = false;
}

// --------------------------------------------------
// snap
// --------------------------------------------------
void Expand::snap() {
    widget->setPosition(reset.x, reset.y);
    widget->scale(reset.w, reset.h);
    stop();
}
