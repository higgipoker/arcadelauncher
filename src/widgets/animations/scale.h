#pragma once

#include "animation.h"

/* scale direction */
enum SCALE_DIRECTION { SCALE_DOWN = -1, SCALE_UP = 1 };

/* Scaling animation */
class Scale : public Animation {
  public:
    /**
     * @brief constructor
     */
    Scale();

    /**
     * @brief Constructor
     * @param target widget to apply animation to
     */
    Scale(Widget *target);

    /**
     * @brief initialize the animation
     * @param s type of scaling
     * @param initial_size scale from
     * @param target_size scale to
     * @param step animation resolution
     */
    void init(SCALE_DIRECTION s, SDL_Rect initial_size, SDL_Rect target_size,
              int step_size);

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

    /// store direction
    SCALE_DIRECTION direction;

    /// track delta change
    int delta;
};