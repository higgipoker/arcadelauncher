#pragma once
#include "animation.h"

/* A Blinking animation for a widget */
class Blink : public Animation {
  public:
    /**
     * @brief Constructor
     */
    Blink();

    /**
     * @brief constructor
     * @param target widgete to apply the animation to
     */
    Blink(Widget *target);

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
};