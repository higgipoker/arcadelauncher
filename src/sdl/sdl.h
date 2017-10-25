#pragma once
#include "../input/joystick.h"
#include "../input/keyboard.h"
#include "text.h"
#include "window.h"
#include <SDL_ttf.h>

class SDL {
  public:
    static void initGraphics();
    static void initSound();
    static void initInput();
    static void close();
    static void clearScreen();
    static void present();
    static SDLWindow *window();
    static SDL_Text *createText(const std::string &font_name, int font_height, bool shadow = false);
    static Input *getInputDevice();
    static bool SOUND_INITED;

  private:
    static SDLWindow *wnd;
    static Joystick *joystick;
    static Keyboard *keyboard;
    static Input *input_device;

    static std::vector<SDL_Text*> text_pointers;
};
