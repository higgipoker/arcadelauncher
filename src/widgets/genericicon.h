#include "icon.h"
#include "../sdl/text.h"

/** @brief  a generic icon to use  when game icon not found (special featuer to
 * add game name text) */
class GenericIcon : public Icon {
  public:
    /**
     * @brief Constructor
     * @param window window context
     * @param name game name
     * @param t text object
     * @param filename the icon file
     * @param border_filename filename of border image
     * @param border_highlight_filename filename of the highlight image
     */
    GenericIcon(SDLWindow *window, const std::string &name,
                const std::string &filename,
                const std::string &border_filename = "",
                const std::string &border_highlight_filename = "");

    ~GenericIcon();

    /**
     * @brief render
     */
    void render();

  protected:
    /// a text object for writing
    SDL_Text *text;
};
