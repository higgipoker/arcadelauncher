#include "slide.h"

#define SLIDE_SPEED 1

// --------------------------------------------------
// Slide
// --------------------------------------------------
Slide::Slide() {
    running = false;
    direction = SLIDE_LEFT;
    target_x = step = 0;
    blocking = true;
}

// --------------------------------------------------
// Slide
// --------------------------------------------------
void Slide::init(SLIDE_DIRECTION dir, int _target_x, int step_size) {
    direction = dir;
    target_x = _target_x;
    step = step_size;
}

// --------------------------------------------------
// Slide
// --------------------------------------------------
Slide::Slide(Widget *target) : Animation(target) {
    target_x = 0;
    direction = SLIDE_LEFT;
    step = 1;
    start();
}

// --------------------------------------------------
// update
// --------------------------------------------------
void Slide::update() {
    if (timer.get_ticks() > SLIDE_SPEED) {

        int delta = step * direction;

        widget->move(delta, 0);
        timer.start();

        switch (direction) {

        case SLIDE_LEFT:
            if (widget->getPosition().x <= target_x) {

                widget->setPosition(target_x, widget->getPosition().y);
                stop();
            }
            break;
        case SLIDE_RIGHT:
            if (widget->getPosition().x >= target_x) {

                widget->setPosition(target_x, widget->getPosition().y);
                stop();
            }
            break;
        }
    }
}

// --------------------------------------------------
// onStart
// --------------------------------------------------
void Slide::onStart() {
    running = true;
    timer.start();
}

// --------------------------------------------------
// onEnd
// --------------------------------------------------
void Slide::onEnd() {
    widget->highlight(false);
    timer.stop();
    running = false;
}

// --------------------------------------------------
// snap
// --------------------------------------------------
void Slide::snap() {
    widget->setPosition(target_x, widget->getPosition().y);
    stop();
}
