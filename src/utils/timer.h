//
//  Timer.h
//  higgilauncher
//
//  Created by Paul Higgins on 29/06/2013.
//  Copyright (c) 2013 Paul Higgins. All rights reserved.
//

#ifndef __higgilauncher__Timer__
#define __higgilauncher__Timer__

#include <iostream>

//  -------------------------------------------------------
//
//  Generic timer
//
//  -------------------------------------------------------
class Timer {

  public:
    // Initializes variables
    Timer();

    // The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    // Gets the timer's time
    int get_ticks();

    // Checks the status of the timer
    bool is_started();
    bool is_paused();

  private:
    // The clock time when the timer started
    int startTicks;

    // The ticks stored when the timer was paused
    int pausedTicks;

    // The timer status
    bool paused;
    bool started;
};

#endif /* defined(__higgilauncher__Timer__) */
