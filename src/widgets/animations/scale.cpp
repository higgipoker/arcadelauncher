#include "scale.h"
#define SCALE_SPEED 5

// --------------------------------------------------
// Scale
// --------------------------------------------------
Scale::Scale() {
    running = false;
    step = direction = SCALE_UP;
    delta = 0;
    blocking = true;
}

// --------------------------------------------------
// Scale
// --------------------------------------------------
Scale::Scale(Widget *target) : Animation(target) {
    running = false;
    step = direction = SCALE_UP;
    delta = 0;
    blocking = true;

    start();
}

// --------------------------------------------------
// init
// --------------------------------------------------
void Scale::init(SCALE_DIRECTION s, SDL_Rect initial_size, SDL_Rect target_size,
                 int step_size) {
    initial = initial_size;
    target = target_size;
    step = 1;
    direction = s;
    delta = 0;
}

// --------------------------------------------------
// update
// --------------------------------------------------
void Scale::update() {
    if (timer.get_ticks() > SCALE_SPEED) {

        delta += step * direction;
        initial.w += delta;
        initial.h += delta;

        widget->scale(initial.w, initial.h);
        timer.start();

        switch (direction) {

        case SCALE_DOWN:
            if (initial.w <= target.w || initial.h <= target.h) {

                widget->scale(target.w, target.h);
                stop();
            }
            break;
        case SCALE_UP:
            if (initial.w >= target.w || initial.h >= target.h) {

                widget->scale(target.w, target.h);
                stop();
            }
            break;

        default:
            break;
        }
    }
}

// --------------------------------------------------
// onStart
// --------------------------------------------------
void Scale::onStart() {
    running = true;
    timer.start();
}

// --------------------------------------------------
// onEnd
// --------------------------------------------------
void Scale::onEnd() {
    widget->highlight(false);
    timer.stop();
    running = false;
}

// --------------------------------------------------
// snap
// --------------------------------------------------
void Scale::snap() {
    widget->scale(target.w, target.h);
    stop();
}
