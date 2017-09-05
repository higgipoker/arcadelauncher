#include "blink.h"

#define BLINK_TIME 500

// --------------------------------------------------
// Blink
// --------------------------------------------------
Blink::Blink() { running = false; }

// --------------------------------------------------
// Blink
// --------------------------------------------------
Blink::Blink(Widget *target) : Animation(target) { start(); }

// --------------------------------------------------
// update
// --------------------------------------------------
void Blink::update() {
    if (timer.get_ticks() > BLINK_TIME) {

        widget->toggleHighlight();
        timer.start();
    }
}

// --------------------------------------------------
// onStart
// --------------------------------------------------
void Blink::onStart() {
    running = true;
    timer.start();
    widget->highlight(true);
}

// --------------------------------------------------
// onEnd
// --------------------------------------------------
void Blink::onEnd() {
    widget->highlight(false);
    timer.stop();
    running = false;
}

// --------------------------------------------------
// snap
// --------------------------------------------------
void Blink::snap() { stop(); }
