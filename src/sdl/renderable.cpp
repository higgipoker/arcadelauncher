//
//  Renderable.cpp
//  sensi2
//
//  Created by Paul Higgins on 28/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//
#include "SDL.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "renderable.h"
#include <stdio.h>

Renderable::Renderable(SDLWindow *window) {
    surface_width = surface_height = 0;
    wnd = window;
    tex = NULL;
    scale_factor_x = scale_factor_y = 0;
}

Renderable::Renderable(SDLWindow *window, const std::string &filename) {
    tex = NULL;
    surface_width = surface_height = 0;
    scale_factor_x = scale_factor_y = 0;
    wnd = window;
    setImage(filename);
}

//  --------------------------------------------------
//  setImage
//  filename : name of the image file to load from
//  --------------------------------------------------

void Renderable::setImage(std::string filename) {
    //  first load to sdl surface
    if (tex != NULL)
        SDL_DestroyTexture(tex);

    SDL_Surface *sfc = 0;
    sfc = IMG_Load(filename.c_str());
    if (sfc == 0) {

        std::cout << SDL_GetError() << ": " << filename << std::endl;

        return;
    }

    //  now create texture for fast rendering
    tex = SDL_CreateTextureFromSurface(wnd->renderer, sfc);
    if (tex == 0) {
        return;
    }

    //  before deleting the temp surface, get the dimensions
    surface_width = sfc->w;
    surface_height = sfc->h;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = sfc->w;
    src_rect.h = sfc->h;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = sfc->w;
    dst_rect.h = sfc->h;

    screen_rect.x = 0;
    screen_rect.y = 0;
    screen_rect.w = sfc->w;
    screen_rect.h = sfc->h;

    //  free up the surface
    SDL_FreeSurface(sfc);

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
}

//  --------------------------------------------------
//  destructor
//  --------------------------------------------------
Renderable::~Renderable() {
    if (tex)
        ;
    SDL_DestroyTexture(tex);
}

//  --------------------------------------------------
//  render
//  --------------------------------------------------
bool Renderable::render() {
    SDL_RenderCopy(wnd->renderer, tex, &src_rect, &dst_rect);
    return true;
}

bool Renderable::render(float scale_x, float scale_y) {
    SDL_Rect r = dst_rect;
    r.w *= scale_x;
    r.h *= scale_y;

    SDL_RenderCopy(wnd->renderer, tex, &src_rect, &r);
    return true;
    SDL_RenderCopy(wnd->renderer, tex, &src_rect, &r);
    return true;

}

bool Renderable::render(int w, int h) {
    SDL_Rect r = dst_rect;
    r.w = w;
    r.h = h;

    SDL_RenderCopy(wnd->renderer, tex, &src_rect, &r);
    return true;
}

bool Renderable::render(const SDL_Rect &target_rect) {
    SDL_RenderCopy(wnd->renderer, tex, &src_rect, &target_rect);
    return true;
}

//  --------------------------------------------------
//  set position
//  --------------------------------------------------
void Renderable::setPosition(int x, int y) {
    dst_rect.x = x;
    dst_rect.y = y;
}

void Renderable::setAlpha(int _alpha) { SDL_SetTextureAlphaMod(tex, _alpha); }
