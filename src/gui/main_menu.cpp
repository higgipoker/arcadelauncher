#include "main_menu.h"
#include "config/config.h"
#include "input/input.h"
#include "resources/resources.h"
#include "sdl/sdl.h"
#include "utils/stringtools.h"

// --------------------------------------------------
// Constructor
// --------------------------------------------------
MainMenu::MainMenu() {
    background = NULL;
    background2 = NULL;
    wnd = NULL;
    selection_theme = NULL;
    snd_choose = NULL;
    snd_mouhaha = NULL;
    inited = false;
    running = false;
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
MainMenu::~MainMenu() {
    delete background;
    delete selection_theme;
}

// --------------------------------------------------
// init
// --------------------------------------------------
void MainMenu::init(SDLWindow *window) {
    wnd = window;

    // only initialize once
    if(!inited) {

        std::cout << Config::data.theme << std::endl;

        if(Config::data.theme == "coverflow") {

            selection_theme = new Coverflow(window);
            selection_theme->init();

        } else if(Config::data.theme == "grid") {

            selection_theme = new StaticGrid(window);
            selection_theme->init();
        }
    }
}

// --------------------------------------------------
// start
// --------------------------------------------------
void MainMenu::start() {
    running = true;

    if(SDL::SOUND_INITED)
        snd_mouhaha = Mix_LoadWAV("sounds/mouhaha.wav");

    if(snd_mouhaha == NULL) {
        std::cout << "Failed to load sound effect: [sounds/mouhaha.wav], "
                  "SDL_mixer Error:"
                  << Mix_GetError() << std::endl;
    }

    if(SDL::SOUND_INITED)
        snd_choose = Mix_LoadWAV("sounds/choose.wav");

    if(snd_choose == NULL) {
        std::cout << "Failed to load sound effect: [sounds/choose.wav], "
                  "SDL_mixer Error:"
                  << Mix_GetError() << std::endl;
    }

    if(SDL::SOUND_INITED)
        Mix_PlayChannel(-1, snd_choose, 0);
}

// --------------------------------------------------
// addGame
// --------------------------------------------------
void MainMenu::addGame(GAME *new_game) {
    // add to theme
    selection_theme->addGame(new_game);

    // add to all games list
    added_games.push_back(new_game->gamename);

    // set the bg
    setBackground(new_game->type);

    background = background2 = getBackground(new_game->type);
}

// --------------------------------------------------
// update
// --------------------------------------------------
void MainMenu::update() {
    if(running) {
        Background *tmp =
            getBackground(selection_theme->getSelectedGame()->type);

        if(tmp != background) {

            tmp->fadeIn();
            background->fadeOut();

            // swap
            background2 = background;
            background = tmp;
        }

        background2->update();
        background->update();
        selection_theme->update();


        background2->update();
        background->update();
        selection_theme->update();
    }
}

// --------------------------------------------------
// render
// --------------------------------------------------
void MainMenu::render() {
    if(running) {

        if(background) {

            background2->render();
            background->render();
        }

        selection_theme->render();
    }
}

// --------------------------------------------------
// handleInput
// --------------------------------------------------
void MainMenu::handleInput(Event event) {
    if(running) {

        switch(event.evt) {

        case UP:
            selection_theme->onUp();
            break;

        case DOWN:
            selection_theme->onDown();
            break;

        case LEFT:
            selection_theme->onLeft();
            break;

        case RIGHT:
            selection_theme->onRight();
            break;

        case SINGLE_TAP:
            Mix_PlayChannel(-1, snd_mouhaha, 0);
            selection_theme->onButton();
            break;

        // avoid warnings...
        case DOUBLE_TAP:
        case FIRE_DOWN:
        case FIRE_LENGTH:
        case FIRE_UP:
        case INPUT_NONE:
        case QUIT:
        case TOTAL_EVENTS:
        default:
            break;
        }
    }
}

// --------------------------------------------------
// stop
// --------------------------------------------------
void MainMenu::stop() {
    if(SDL::SOUND_INITED) {
        Mix_FreeChunk(snd_choose);
        Mix_FreeChunk(snd_mouhaha);
    }
}

// --------------------------------------------------
// launchGame
// --------------------------------------------------
void MainMenu::launchGame(const std::string &rom_name) {}

// --------------------------------------------------
// setBackground
// --------------------------------------------------
void MainMenu::setBackground(PLATFORM _platform) {

    for(auto it = backgrounds.begin(); it != backgrounds.end(); ++it) {
        if((*it).first == _platform) {
            return;
        }
    }

    Background *bg = new Background(wnd, Resources::GetBackgroundName(_platform));
    backgrounds.push_back(std::make_pair(_platform, bg));
}

// --------------------------------------------------
// getBackground
// --------------------------------------------------
Background *MainMenu::getBackground(PLATFORM _platform) {
    for(auto it = backgrounds.begin(); it != backgrounds.end(); ++it) {
        if((*it).first == _platform) {
            return (*it).second;
        }
    }

    return NULL;
}
