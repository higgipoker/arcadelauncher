#ifndef EMULATOR_H
#define EMULATOR_H

#include "input.h"

class Emulator : public Input {
  public:
    Emulator();

  private:
    void pressButton();
    void releaseButton();
};

#endif // EMULATOR_H
