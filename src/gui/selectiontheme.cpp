#include "selectiontheme.h"
#include "../sdl/sdl.h"

// --------------------------------------------------
// constructor
// --------------------------------------------------
SelectionTheme::SelectionTheme(SDLWindow *wnd) {
    window = wnd;

    snd_move = NULL;
    returning_from_game = false;

    if (SDL::SOUND_INITED)
        snd_move = Mix_LoadWAV("sounds/test.wav");

    if (snd_move == NULL) {

        std::cout << "Failed to load sound effect: [sounds/test.wav], SDL_mixer Error:"
                  << Mix_GetError() << std::endl;
    }

    snd_select = NULL;

    if (SDL::SOUND_INITED)
        snd_select = Mix_LoadWAV("sounds/select.wav");

    if (snd_select == NULL) {

        std::cout << "Failed to load sound effect: [sounds/select.wav], SDL_mixer Error:"
                  << Mix_GetError() << std::endl;
    }

    if (SDL::SOUND_INITED)
        snd_bg = Mix_LoadWAV("sounds/bg.wav");

    if (snd_bg == NULL) {

        std::cout
            << "Failed to load sound effect: [sounds/bg.wav], SDL_mixer Error:"
            << Mix_GetError() << std::endl;
    }

    if (SDL::SOUND_INITED)
        Mix_PlayChannel(2, snd_bg, -1);
}

// --------------------------------------------------
// getSelectedGame
// --------------------------------------------------
GAME *SelectionTheme::getSelectedGame(void) {    
    return games[getSelectionIndex()];
}

// --------------------------------------------------
// destructor
// --------------------------------------------------
SelectionTheme::~SelectionTheme() {
    if (SDL::SOUND_INITED) {
        Mix_FreeChunk(snd_move);
        Mix_FreeChunk(snd_select);
        Mix_FreeChunk(snd_bg);
    }
}
