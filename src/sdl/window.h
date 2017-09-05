//
//  SDL_Window.h
//  sensi2
//
//  Created by Paul Higgins on 19/07/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#ifndef __sensi2__SDL_Window__
#define __sensi2__SDL_Window__

#include <iostream>

#include "SDL.h"

#include "utils/color.h"

class SDLWindow {

  public:
    /* intiialise the window */
    void init(std::string title, int x_pos, int y_pos, int wnd_width,
              int wnd_height, unsigned int flag);

    /* present back buffer */
    void present();

    /* clear screen */
    void clear(Color &c);

    /* set render color for primitives */
    void setRenderColor(Color &c);

    /* clean up */
    void close();

    /* sdl renderer */
    SDL_Renderer *renderer;

    /* sdl window */
    SDL_Window *win;

  protected:

};

#endif /* defined(__sensi2__SDL_Window__) */
