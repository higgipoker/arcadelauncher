#pragma once
#include "../resources/resources.h"
#include "../sdl/window.h"
#include "../widgets/widget.h"
#include <SDL_mixer.h>
#include <vector>

/**
 * @brief an absttract class -> interface for selection SelectionTheme
 */
class SelectionTheme {
  public:
    /**
     * @brief constructor
     */
    SelectionTheme(SDLWindow *wnd);

    /**
     * @brief destructor
     */
    virtual ~SelectionTheme();

    /**
     *@brief initialize the SelectionTheme
     */
    virtual void init() = 0;

    /**
     * @brief add a game to the menu
     */
    virtual void addGame(GAME *new_game) = 0;

    /**
     * @brief get the currently selected game getSelectionIndex
     */
    virtual int getSelectionIndex() = 0;

    /**
     * @brief poll method
     */
    virtual void update() = 0;

    /**
     * @brief render to screen
     */
    virtual void render() = 0;

    /**
     * @brief input left
     */
    virtual void onLeft() = 0;

    /**
     * @brief input right
     */
    virtual void onRight() = 0;

    /**
     * @brief input up
     */
    virtual void onUp() = 0;

    /**
     * @brief input down
     */
    virtual void onDown() = 0;

    /**
     * @brief input button pressed
     */
    virtual void onButton() = 0;

    /**
    * @brief run the selected game
    */
    virtual void launchGame() = 0;

    GAME *getSelectedGame(void);

    bool returning_from_game;

  protected:
    /* list of icons */
    std::vector<GAME *> games;

    /* for rendering */
    SDLWindow *window;

    /* sound effects */
    Mix_Chunk *snd_bg;
    Mix_Chunk *snd_move;
    Mix_Chunk *snd_select;
};
