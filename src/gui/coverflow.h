#pragma once
#include "../sdl/window.h"
#include "selectiontheme.h"
#include "../widgets/animations/animation.h"
#include "../widgets/animations/blink.h"
#include "../widgets/animations/expand.h"
#include "../widgets/animations/scale.h"
#include "../widgets/animations/slide.h"
#include "../widgets/video.h"
#include "../widgets/widget.h"
#include <vector>

class Coverflow : public SelectionTheme {
public:
    // --------------------------------------------
    // CONSTRUCTOR / DESTRUCTOR
    // --------------------------------------------
    Coverflow();
    Coverflow(SDLWindow *wnd);
    ~Coverflow();
    // --------------------------------------------

    // --------------------------------------------
    // SELECTION THEME PURE VIRTUAL
    // --------------------------------------------
    virtual void init() {
        /* DO NOTHING */
    }
    virtual void onLeft();
    virtual void onRight();
    virtual void onUp() {
        /* DO NOTHING */
    };
    virtual void onDown() {
        /* DO NOTHING */
    };
    virtual void onButton();
    virtual void launchGame();
    virtual void render();
    virtual void update();
    virtual void addGame(GAME *new_game);
    virtual int getSelectionIndex();
    // --------------------------------------------

protected:
    /* do the shift left animation */
    void shiftLeft();

    /* do the shift right animation */
    void shiftRight();

    /* add an animation to the list */
    void addAnimation(Animation *anim);

    /* calculate screen positions for all icons */
    void calcIconIndexes();

    /* calc the dimensions of the scroll marker */
    void calcMarkerDimensions();

    /* move the scroll maker left */
    void markerLeft();

    /* move the scroll marker right */
    void markerRight();

    /* update the video preview window */
    void updateVideo(bool force_change = false);

    /* create a texture for the scrollbar bg */
    void createScrollbar(int x, int y, int w, int h);

    /* render the scrollbar bg */
    void renderScrollbar();

    /* helper to set up the video */
    void configureVideo();

    /* store the animations */
    std::vector<Animation *> animations;

    /* all the animations for this theme */
    Blink blink_animation;
    Slide slide_left;
    Slide slide_center;
    Slide slide_right;
    Slide slide_offscreen;
    Scale scale_left;
    Scale scale_center;
    Scale scale_right;
    Scale scale_offscreen;
    Expand expand;

    /* store the index of which icons occupy which positions in the layout */
    int left_offscreen_index, left_index, center_index, right_index, right_offscreen_index;

    /* position and dimensions for each slot in the layout */
    SDL_Rect icon_highlight_position;
    SDL_Rect icon_left_position;
    SDL_Rect icon_right_position;
    SDL_Rect icon_selected_position;
    SDL_Rect icon_left_offscreen_position;
    SDL_Rect icon_right_offscreen_position;
    SDL_Rect video_position;
    SDL_Rect scrollbar_position;
    SDL_Rect scrollbar_rect;

    /* store positions of the scroll marker */
    std::vector<SDL_Rect> marker_positions;

    /* gameflow/logic helpers */
    bool launch_pending;
    bool waiting_for_animation;
    int pending_offset;
    bool pending_right;
    bool pending_left;
    int gap;

    /* the scroll marker widget */
    Widget *marker;
    SDL_Texture *scrollbar;

    /* current marker position index */
    int marker_index;
    int MARKER_Y;

    /* video preview */
    Video *video;

    /* track to see if video stream needs changed */
    int last_video_index;
};
