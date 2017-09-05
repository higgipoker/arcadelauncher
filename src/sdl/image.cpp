#include "image.h"

Image::Image(SDLWindow *window, const std::string &filename)
    : Renderable(window, filename) {}
