#pragma once

#include "sdl/image.h"
#include "utils/point.h"
#include "widget.h"

/** @brief Background widget */
class Background : public Widget {
  public:
    /**
     * @brief Constructor
     * @param window target window
     * @param filename image file
     */
    Background(SDLWindow *window, const std::string &filename);

    /**
     * @brief Destructor
     */
    virtual ~Background();

    /**
     * @brief render
     */
    virtual void render();

    /*
     * @brief set positions
     * @param _x new x
     * @param _y new y
     */
    virtual void setPosition(int _x, int _y);

    /*
     * @brief get position
     */
    virtual Point getPosition() { return img->getPosition(); };

    /**
     * @brief get width
     */
    virtual int getWidth() { return img->getWidth(); };

    /**
     * @brief get higght
     */
    virtual int getHeight() { return img->getHeight(); };

  protected:
    /// bg image
    Image *img;
};
