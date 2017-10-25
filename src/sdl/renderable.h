//
//  Renderable.h
//  sensi2
//
//  Created by Paul Higgins on 28/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "../utils/point.h"
#include "window.h"

//  -------------------------------------------------------
//
//  Something that can be rendered on the screen
//
//  -------------------------------------------------------

class Renderable {

  public:
    Renderable(SDLWindow *window);

    Renderable(SDLWindow *window, const std::string &filename);

    /* main render method */
    virtual bool render();
    virtual bool render(float scale_x, float scale_y);
    virtual bool render(int w, int h);
    virtual bool render(const SDL_Rect &target_rect);

    /*  destuctor */
    virtual ~Renderable();

    /*  set the image file for rendering  */
    virtual void setImage(std::string filename);

    /* set position intuitively */
    virtual void setPosition(int x, int y);

    /* get position intuitively */
    virtual Point getPosition() { return Point(dst_rect.x, dst_rect.y); }

    /* get width */
    virtual int getWidth(void) { return src_rect.w; }

    /* get height */
    virtual int getHeight(void) { return src_rect.h; }

    //  section of image file to render from
    SDL_Rect src_rect;

    //  destination rect to render to
    SDL_Rect dst_rect;

    //  translated screen position
    SDL_Rect screen_rect;

    //  teture for rendering
    SDL_Texture *tex;

    //  save width and height of surface
    int surface_width;
    int surface_height;

    void setAlpha(int _alpha);

  protected:
    SDLWindow *wnd;
    float scale_factor_x;
    float scale_factor_y;
};
