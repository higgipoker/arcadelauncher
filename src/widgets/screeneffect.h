#pragma once

#include "../sdl/image.h"
#include "widget.h"

class ScreenEffect : public Widget {
  public:
    ScreenEffect(SDLWindow *wnd, const std::string &filename, int width,
                 int height);
    ~ScreenEffect();

    /**
     * @brief render
     */
    virtual void render();

    /**
     * @brief render
     * @param target_rect render to rect
     */
    virtual bool render(const SDL_Rect &target_rect);

    /**
     * @brief blend
     * @param alpha new alpha
     */
    virtual void blend(int alpha);

    /**
     * @brief move
     * @param _x new x
     * @param _y new y
     */
    virtual void move(int _x, int _y);

    /**
     * @brief set getPosition
     * @param _x new x
     * @param _y new y
     */
    virtual void setPosition(int _x, int _y);

    /**
     * @brief get get position
     */
    virtual Point getPosition() { return Point(); };

    /**
     * @brief get width
     */
    virtual int getWidth();

    /**
     * @brief get height
     */
    virtual int getHeight();

  protected:
    SDLWindow *window;
    SDL_Texture *overlay;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    bool flicker;

    int intensity;
    int flicker_intensity;
};
