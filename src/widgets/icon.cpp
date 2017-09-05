
#include "icon.h"

// --------------------------------------------------
// Icon
// --------------------------------------------------
Icon::Icon(SDLWindow *window, const std::string &filename,
           const std::string &border_filename,
           const std::string &border_highlight_filename) {
    x = y = 0;

    image = new Image(window, filename);

    if (border_filename != "")
        border = new Image(window, border_filename);
    else
        border = NULL;

    if (border_highlight_filename != "")
        border_lit = new Image(window, border_highlight_filename);
    else
        border_lit = NULL;
}

// --------------------------------------------------
// ~Icon
// --------------------------------------------------
Icon::~Icon() {
    delete border;
    delete border_lit;
    delete image;
}

// --------------------------------------------------
// render
// --------------------------------------------------
void Icon::render() {
    if (visible) {
        if (size_h || size_w) {
            image->render(size_w, size_h);

            if (lit) {
                if (border_lit)
                    border_lit->render(size_w, size_h);
            } else {
                if (border)
                    border->render(size_w, size_h);
            }
        } else {
            image->render();
        }
    }
}

// --------------------------------------------------
// render
// --------------------------------------------------
bool Icon::render(const SDL_Rect &target_rect) {
    if (visible)
        return image->render(target_rect);

    return false;
}

// --------------------------------------------------
// setPosition
// --------------------------------------------------
void Icon::setPosition(int _x, int _y) {
    x = _x;
    y = _y;

    if (border)
        border->setPosition(x, y);
    if (border_lit)
        border_lit->setPosition(x, y);
    image->setPosition(x, y);
}

// --------------------------------------------------
// blend
// --------------------------------------------------
void Icon::blend(int alpha) { SDL_SetTextureAlphaMod(image->tex, alpha); }

// --------------------------------------------------
// move
// --------------------------------------------------
void Icon::move(int _x, int _y) {
    if (border)
        border->setPosition(image->getPosition().x + _x,
                            image->getPosition().y + _y);
    if (border_lit)
        border_lit->setPosition(image->getPosition().x + _x,
                                image->getPosition().y + _y);

    image->setPosition(image->getPosition().x + _x,
                       image->getPosition().y + _y);
}

// --------------------------------------------------
// getWidth
// --------------------------------------------------
int Icon::getWidth() { return size_w; }

// --------------------------------------------------
// getHeight
// --------------------------------------------------
int Icon::getHeight() { return size_h; }
