#pragma once

#include "../sdl/window.h"
#include "selectiontheme.h"
#include "../utils/gridtools.h"
#include "../widgets/animations/blink.h"
#include "../widgets/animations/expand.h"
#include "../widgets/icon.h"
#include "../widgets/video.h"
#include "../widgets/widget.h"
#include <vector>

class StaticGrid : public SelectionTheme {
  public:
    // --------------------------------------------
    // CONSTRUCTOR / DESTRUCTOR
    // --------------------------------------------
    StaticGrid(SDLWindow *wnd);

    virtual ~StaticGrid();
    // --------------------------------------------

    // --------------------------------------------
    // SELECTION THEME PURE VIRTUAL
    // --------------------------------------------
    virtual void init();
    virtual void addGame(GAME *new_game);
    virtual int getSelectionIndex();
    virtual void update();
    virtual void render();
    virtual void onLeft();
    virtual void onRight();
    virtual void onUp();
    virtual void onDown();
    virtual void onButton();
    virtual void launchGame();
    // --------------------------------------------

  protected:
    /* active selection animation */
    Blink blink_animation;

    /* a grid manager */
    GridManager grids;

    /* list of dynamic animations */
    std::vector<Animation *> animations;

    void configureVideo();

    /* preview video */
    Video *preview_video;

    /* track video */
    int last_video_index;

    /* track the currently selected game */
    int selection_index;

    /* helper */
    bool animations_finished;

    /* helper methods */
    void resetPositions();
    void animateGridsLeft();
    void animateGridsRight();
    bool iconVisible(int index);
    void updateVideo();
    void createLeftMarker();
    void createRightMarker();

    /* to execute after animations */
    bool pending_grid_left;
    bool pending_grid_right;
    void completeGridLeft();
    void completeGridRight();
    bool launch_pending;

    /* animation for the game launch anim */
    Expand expand;

    /* data for the "more games" markers */
    SDL_Texture *marker_left;
    SDL_Texture *marker_right;
    SDL_Rect scrollbar_rect_left;
    SDL_Rect scrollbar_rect_right;
    SDL_Rect scrollbar_position_left;
    SDL_Rect scrollbar_position_right;

    /* track current page */
    int current_page;

    Mix_Chunk *snd_tick;
};
