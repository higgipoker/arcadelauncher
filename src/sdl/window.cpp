//
//  SDL_Window.cpp
//  sensi2
//
//  Created by Paul Higgins on 19/07/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#include "window.h"
#include <SDL.h>

//  -------------------------------------------------------
//  init
//  -------------------------------------------------------
void SDLWindow::init(std::string title, int x_pos, int y_pos, int wnd_width, int wnd_height, unsigned int flags) {
    win = SDL_CreateWindow(title.c_str(), x_pos, y_pos, wnd_width, wnd_height, (Uint32)flags);

    if (win == 0) {

        std::cout << "Could not create SDL window" << std::endl;
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == 0) {
        std::cout << "Could not create SDL renderer" << std::endl;
    }

    SDL_RenderSetLogicalSize(renderer, 800, 600);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

//  --------------------------------------------------
//  set render color
//  --------------------------------------------------
void SDLWindow::setRenderColor(Color &c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

//  --------------------------------------------------
//  clear
//  --------------------------------------------------
void SDLWindow::clear(Color &c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(renderer);
}

//  --------------------------------------------------
//  present
//  --------------------------------------------------
void SDLWindow::present() {
    SDL_RenderPresent(renderer);
}

//  --------------------------------------------------
//  close
//  --------------------------------------------------
void SDLWindow::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
}
