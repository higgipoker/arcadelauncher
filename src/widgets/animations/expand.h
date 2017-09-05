#pragma once

#include "animation.h"

/* Scaling animation */
class Expand : public Animation {
  public:
    /**
     * @brief constructor
     */
    Expand();

    /**
     * @brief Constructor
     * @param target widget to apply animation to
     */
    Expand(Widget *target);

    /**
     * @brief initialize the animation
     * @param s type of scaling
     * @param initial_size scale from
     * @param target_size scale to
     * @param step animation resolution
     */
    void init(SDL_Rect initial_size, SDL_Rect target_size, int step_size);

    /**
      * @brief polymorphic on start behaviour
      */
    virtual void onStart();

    /**
     * @brief polymorphic on start behaviour
     */
    virtual void onEnd();

    /**
     * @brief polymorphic on start behaviour
     */
    virtual void update();

    /**
     * @brief polymorphic on start behaviour
     */
    virtual void snap();

  protected:
    /// store initial size
    SDL_Rect initial;

    /// store target size
    SDL_Rect target;

    /// store step size
    int step;

    /// track delta change
    int delta;

    int scale_x_step;
    int scale_y_step;

    int offset_x;
    int offset_y;

    SDL_Rect reset;

    Timer end_delay;
    bool end_pending;
};
