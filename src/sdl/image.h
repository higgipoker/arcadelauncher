#pragma once

#include "renderable.h"

class Image : public Renderable {
  public:
    Image(SDLWindow *window, const std::string &filename);

  protected:
};
