#ifndef SDL_TEXT_H
#define SDL_TEXT_H

#include "renderable.h"
#include "window.h"
#include <SDL_ttf.h>

class SDL_Text : public Renderable {
  public:
    SDL_Text(SDLWindow *window, const std::string &font_name, int font_height,
             const std::string &text, bool _shadow = false);
    SDL_Text(SDLWindow *window, const std::string &font_name, int font_height,
             bool _shadow = false);
    virtual ~SDL_Text();

    bool render();
    void setText(const std::string &text);
    void setText(int i);

    void setColor(SDL_Color c) { color = c; }

    /* get position intuitively */
    virtual Point getPosition() { return Point(dst_rect.x, dst_rect.y); }

    /* get width */
    virtual int getWidth(void) { return src_rect.w; }

    /* get height */
    virtual int getHeight(void) { return src_rect.h; }

    /* set position intuitively */
    virtual void setPosition(int x, int y);

    bool shadow;

    bool hud;

    //    /* get position intuitively */
    //    virtual Point getPosition();

    //    /* get width */
    //    virtual int getWidth ( void );

    //    /* get height */
    //    virtual int getHeight ( void );

  private:
    SDL_Color color;
    SDL_Color color_shadow;
    TTF_Font *font;
    SDL_Texture *shadow_texture;

    SDL_Point save_position;
    SDL_Rect shadow_rect;

    std::string t;
};

#endif // SDL_TEXT_H
