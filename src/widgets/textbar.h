#include "sdl/text.h"
#include "utils/timer.h"
#include "widget.h"

class TextBar : public Widget {
  public:
    TextBar(SDLWindow *window, const std::string &text, int w, int h);
    virtual ~TextBar();
    void render();

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

    virtual void show();

    /**
     * @brief get width
     */
    virtual int getWidth();

    /**
     * @brief get height
     */
    virtual int getHeight();

  protected:
    SDL_Text *text;
    int width;
    int height;
    SDLWindow *wnd;
};
