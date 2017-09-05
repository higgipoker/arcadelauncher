#pragma once
#include "animation.h"

/* A Pulseing animation for a widget */
class Pulse : public Animation {
  public:
    /**
     * @brief Constructor
     */
    Pulse();

    /**
     * @brief constructor
     * @param target widgete to apply the animation to
     */
    Pulse(Widget *target);

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