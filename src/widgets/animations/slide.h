#pragma once

#include "animation.h"

/* direction */
enum SLIDE_DIRECTION { SLIDE_LEFT = -1, SLIDE_RIGHT = 1 };

/** @brief A slide animation */
class Slide : public Animation {
  public:
    /**
     * @brief Constructor
     */
    Slide();

    /**
     * @brief constructor
     * @param target widget to apply animation to
     */
    Slide(Widget *target);

    /**
     * @brief init the animation
     * @param dir direction of animation
     * @param target_x target x
     * @param target_y target y
     * @param step_size resolution
     */
    void init(SLIDE_DIRECTION dir, int target_x, int step_size);

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
    /// move direction
    SLIDE_DIRECTION direction;

    /// target x
    int target_x;

    /// anim resolution
    int step;
};