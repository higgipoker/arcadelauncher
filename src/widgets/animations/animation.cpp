#include "animation.h"

// --------------------------------------------------
// Animation
// --------------------------------------------------
Animation::Animation() {
    running = false;
    blocking = false;
    widget = NULL;
}

// --------------------------------------------------
// Animation
// --------------------------------------------------
Animation::Animation(Widget *target) {
    widget = target;
    running = false;
    blocking = false;
}

// --------------------------------------------------
// start
// --------------------------------------------------
void Animation::start() {
    onStart();
    running = true;
}

// --------------------------------------------------
// stop
// --------------------------------------------------
void Animation::stop() {
    onEnd();
    running = false;
}

// --------------------------------------------------
// changeTarget
// --------------------------------------------------
void Animation::changeTarget(Widget *target) {
    if (running) {

        onEnd();
    }

    widget = target;

    onStart();
}

// --------------------------------------------------
// ~Animation
// --------------------------------------------------
Animation::~Animation() {}