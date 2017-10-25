#pragma once

#include <memory>
#include <iostream>

#include "../sdl/image.h"
#include "widget.h"

/** @brief Icon widget */
class Icon : public Widget {
  public:
    /**
     * @brief Constructor
     * @param window window context
     * @param filename image file name
     * @param border_filename image border
     * @param border_highlight_filename highlight filename
     */
    Icon(SDLWindow *window, const std::string &filename, const std::string &border_filename = "",
         const std::string &border_highlight_filename = "");

    /**
     * @brief Destructor
     */
    ~Icon();

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
     * @brief set getPosition
     * @param _x new x
     * @param _y new y
     */
    virtual void setPosition(int _x, int _y);

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
     * @brief get get position
     */
    virtual Point getPosition() {
        return image->getPosition();
    }

    /**
     * @brief get width
     */
    virtual int getWidth();

    /**
     * @brief get height
     */
    virtual int getHeight();

    std::string launcher;
    std::string gamename;

  protected:
    /// border image
    std::unique_ptr<Image> border;

    /// highlight border image
    std::unique_ptr<Image> border_lit;

    /// icon image    
    std::unique_ptr<Image> image;

    /// position
    int x, y;
};
