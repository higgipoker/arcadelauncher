#include "genericicon.h"
#include "../sdl/sdl.h"

// --------------------------------------------------
// GenericIcon
// --------------------------------------------------
GenericIcon::GenericIcon(SDLWindow *window, const std::string &name, const std::string &filename,
                         const std::string &border_filename,
                         const std::string &border_highlight_filename)
    : Icon(window, filename, border_filename, border_highlight_filename) {
    text = SDL::createText("fonts/atari full.ttf", 12, SDL::window());
    text->setText(name);
}

GenericIcon::~GenericIcon(){

}
// --------------------------------------------------
// render
// --------------------------------------------------
void GenericIcon::render() {
    Icon::render();

    if (visible) {

        if (size_h || size_w) {

            text->setPosition(
                this->image->dst_rect.x + (size_w / 2) - text->getWidth() / 2,
                this->image->dst_rect.y + (size_h / 2) - text->getHeight() / 2);
            text->render();
        } else {
            text->setPosition(this->image->dst_rect.x + (size_w / 2),
                              this->image->dst_rect.y + (size_h / 2));
            text->render();
        }
    }
}
