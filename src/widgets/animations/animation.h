#pragma once

#include "../../utils/timer.h"
#include "../widget.h"

/* Abstract base class for widget animations */
class Animation {
  public:
    /**
     * @brief Default Constructor
     */
    Animation();

    /**
     * @brief Constructor
     * @param target widget to apply this animation to
     */
    Animation(Widget *target);

    /**
     * @brief Destructor
     */
    virtual ~Animation();

    /**
     * @brief start the animation
     */
    virtual void start();

    /**
     * @brief polymorphic on start behaviour
     */
    virtual void onStart() = 0;

    /**
     * @brief polymorphic on end behaviour
     */
    virtual void onEnd() = 0;

    /**
     * @brief polymorphic update behaviour
     */
    virtual void update() = 0;

    /**
     * @brief polymorphic snap behaviour \
     *              changes widget immediately to end condition \
     *              and stops the animation
     */
    virtual void snap() = 0;

    /**
     * @brief stop the animation
     */
    virtual void stop();

    /**
     * @brief apply this animation to a widget
     * @param target widget to apply this animation to
     */
    void changeTarget(Widget *target);

    /// is the animation still running?
    bool running;

    /// eg a transition anim sets this to true, a never ending loop animation
    /// sets it to false
    bool blocking;

  protected:
    /// the target widget
    Widget *widget;

    /// animation timer
    Timer timer;
};
