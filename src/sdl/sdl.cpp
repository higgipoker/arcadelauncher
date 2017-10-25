#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>

#include "../config/config.h"
#include "../utils/stringtools.h"

#include "sdl.h"

SDLWindow *SDL::wnd = NULL;
Keyboard *SDL::keyboard = NULL;
Joystick *SDL::joystick = NULL;
Input *SDL::input_device = NULL;
std::vector<SDL_Text*> SDL::text_pointers;

bool SDL::SOUND_INITED = false;

void SDL::initGraphics() {
    SOUND_INITED = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {

        std::cout << (SDL_GetError()) << std::endl;
    }

    if (TTF_Init() == -1) {

        std::cout << (SDL_GetError()) << std::endl;
    }

    // Declare display mode structure to be filled in.
    SDL_DisplayMode current;

    SDL_Init(SDL_INIT_VIDEO);

    // Get current display mode of all displays.
    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {

        int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

        if (should_be_zero != 0)
            // In case of error...
            SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

        else
            // On success, print the current display mode.
            SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h,
                    current.refresh_rate);
    }

    // screen dimensions
    unsigned int WINDOW_WIDTH = StringTools::toInt(Config::data.screen_width);
    unsigned int WINDOW_HEIGHT = StringTools::toInt(Config::data.screen_height);
    unsigned int WINDOW_X = StringTools::toInt(Config::data.screen_offset_x);
    unsigned int WINDOW_Y = StringTools::toInt(Config::data.screen_offset_y);

    unsigned int WINDOW_FLAGS = 0;

    if (Config::data.screen_mode == "fullscreen") {
        std::cout << "Fullscreen" << std::endl;
        WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN;
        WINDOW_WIDTH = current.w;
        WINDOW_HEIGHT = current.h;
    }

    // create sdl window context
    wnd = new SDLWindow();
    wnd->init("Launcher", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);

    // keep focus
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

// logical size
// SDL_RenderSetLogicalSize( wnd->renderer, WINDOW_WIDTH, WINDOW_HEIGHT );

// hide cursor
#ifdef NDEBUG
    SDL_ShowCursor(false);
#endif

    SDL_Log("%s%d", "initGraphics: Window display index: ", SDL_GetWindowDisplayIndex(wnd->win));
}

void SDL::initSound() {
    SDL_Log("%s", "initSound...");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    SDL_Log("%s", "initSound: done");

    SOUND_INITED = true;
    Mix_Volume(-1, 80);   
}

void SDL::initInput() {
    // a joystick object
    joystick = new Joystick();

    // a keyboard object
    keyboard = new Keyboard();

    // initialize a joystick
    if (SDL_NumJoysticks() > 0) {
        joystick->init();

    } else {
        std::cout << ("No joysticks detected") << std::endl;
        delete joystick;
        joystick = NULL;
    }

    if (joystick) {
        input_device = joystick;

    } else {
        input_device = keyboard;
    }

    SDL_Log("%s", "initInput: done");
}

void SDL::close() {
    for(auto it=text_pointers.begin(); it!=text_pointers.end(); ++it){
        delete *it;
    }
    SDL_ShowCursor(true);
    wnd->close();

    if (SDL::SOUND_INITED)
        Mix_HaltMusic();

    TTF_Quit();
    SDL_Quit();
    delete wnd;
    delete joystick;
    delete keyboard;
}

void SDL::clearScreen() {
    SDL_SetRenderDrawColor(wnd->renderer, 0, 0, 0, 255);
    SDL_RenderClear(wnd->renderer);
}

void SDL::present() {
    wnd->present();
}

SDL_Text *SDL::createText(const std::string &font_name, int font_height, bool shadow) {
    // game text object using the font
    SDL_Text *text = new SDL_Text(wnd, font_name, font_height);

    // init shadow
    text->shadow = shadow;

    text_pointers.push_back(text);

    return text;
}

SDLWindow *SDL::window() {
    return wnd;
}

Input *SDL::getInputDevice() {
    return input_device;
}
