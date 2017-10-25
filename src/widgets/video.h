#pragma once

#include "icon.h"
#include "../sdl/window.h"
#include "../utils/timer.h"
#include "../video/videostreamer2.h"
#include "widget.h"

class Video : public Widget {
  public:
    /**
    * @brief Constructor
    * @param window window context
    * @param filename image file name
    * @param border_filename image border
    * @param border_highlight_filename highlight filename
    */
    Video(SDLWindow *window, bool with_border);

    /**
     * @brief set the video source file
     * @param filename video file
     */
    void setStreamSource(const std::string &filename);

    /**
     * @brief stop the video playback
     */
    void stop();

    /**
     * @brief Destructor
     */
    ~Video();

    /**
     * @brief render
     */
    virtual void render();

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
    virtual Point getPosition();

    /**
     * @brief get width
     */
    virtual int getWidth();

    /**
     * @brief get height
     */
    virtual int getHeight();

    /**
     * @brief scale
     */
    virtual void scale(int _w, int _h);

  protected:
    std::unique_ptr<VideoStreamer2> stream;

    SDL_Rect videoRectSrc;
    SDL_Rect videoRectDst;
    SDLWindow *window;
    SDL_Texture *video_texture;
    Timer frame_timer;
    std::string video_source;
    void clearScreen();

    bool stream_started;

    /**
     * @brief helper to create sdl surface
     */
    void createTexture();    
    std::unique_ptr<Icon> border;

    int border_width;
};
