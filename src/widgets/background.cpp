#include "background.h"

// --------------------------------------------------
// Background
// --------------------------------------------------
Background::Background(SDLWindow *window, const std::string &filename) {
    img = new Image(window, filename);
    alpha = 255;
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
Background::~Background() { delete img; }

// --------------------------------------------------
// render
// --------------------------------------------------
void Background::render() {
    if (visible) {
        img->setAlpha(alpha);
        if (size_h || size_w) {

            img->render(size_w, size_h);
        } else {
            img->render();
        }
    }
}

// --------------------------------------------------
// setPosition
// --------------------------------------------------
void Background::setPosition(int _x, int _y) { img->setPosition(_x, _y); }
