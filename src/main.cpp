/* ffmpeg special includes */
#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#include "utils/ipaddress.h"
#include "utils/stringtools.h"
#include "widgets/textbar.h"

/* own sdl wrapper */
#include "sdl/sdl.h"

/* configuration tools */
#include "config/config.h"

/* resource tools */
#include "resources/resources.h"

/* main menu */
#include "gui/main_menu.h"

/* the main menu */
static MainMenu main_menu;

/* quit program? */
static bool quit = false;

/* game for single game mode */
static GAME *single_game;

static TextBar *ip_bar;

// --------------------------------------------------
// handleInput
// --------------------------------------------------
void handleInput() {
    // normal sdl event
    SDL_Event event;

    // event to be handled by main menu
    Event menu_event;

    // avoid double launches
    if (main_menu.returningFromGame()) {
        // reset all input states
        SDL::getInputDevice()->reset();

        // resume main menu handling
        main_menu.resume();

        // killall sdl events
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    } else {
        // sdl event polling
        SDL_PollEvent(&event);

        // convert to input event
        menu_event = SDL::getInputDevice()->getInputEvent(event);

        // handle the input
        main_menu.handleInput(menu_event);

        if (event.type == SDL_KEYDOWN) {

            switch (event.key.keysym.sym) {

                case SDLK_TAB:
                    ip_bar->show();
                    break;

                default:
                    break;
            }
        }
    }

#ifndef NDEBUG
    // special quit event
    if (menu_event.evt == QUIT) {
        quit = true;
    }
#endif
}

// --------------------------------------------------
// addGames
// --------------------------------------------------
void addGames() {
    // just add all games for now
    for (unsigned int i = 0; i < Resources::game_list.size(); i++) {
        main_menu.addGame(Resources::game_list[i]);
    }
}

// --------------------------------------------------
// checkSingleGameMode
// --------------------------------------------------
bool checkSingleGameMode(void) {
    if (!Config::data.single_game.empty()) {
        single_game = Resources::GetGame(Config::data.single_game);

        if (single_game->gamename != "unknown") {
            return true;

        } else {
            std::cout << "WARNING: single mode set but game was not found" << std::endl;
        }
    }

    return false;
}

// --------------------------------------------------
// doSingleGame
// --------------------------------------------------
void doSingleGame(void) {
    int s = system(single_game->exec_command.c_str());

    if (s != 0) {
        std::cout << "SYSTEM CALL FAILED WITH CODE " << s << std::endl;
    }
}

// --------------------------------------------------
// mainLoop
// --------------------------------------------------
void mainLoop() {
    // get input
    handleInput();

    // update menu
    main_menu.update();

    // clear screen
    SDL::clearScreen();

    // render menu
    main_menu.render();

    // render screen effect
    Resources::RenderScreenEffect();

    // render the ip address bar
    ip_bar->render();

    // update screen
    SDL::present();
}

// --------------------------------------------------
// main
// --------------------------------------------------
int main(int argc, char *argv[]) {
    // extern "C" int main(/*int argc, char *argv[]*/) {

    // open the config file
    Config::data.Open("data/launcher.cfg");

    // init SDL stuff
    SDL::initGraphics();
    SDL::initSound();
    SDL::initInput();

    // init ffmpeg
    av_register_all();

    // init the resources
    Resources::Init();

    // check for single game mode
    if (checkSingleGameMode()) {
        doSingleGame();
    }

    // still possible to exit to menu in single game mode

    // make a main menu
    main_menu.init(SDL::window());

    // add games to menu
    addGames();

    // start main menu
    main_menu.start();

    std::string ip = IPAddress::Get();
    std::cout << ip << std::endl;

    ip_bar =
        new TextBar(SDL::window(), ip, StringTools::toInt(Config::data.screen_width), 42);

    // main program loop
    while (!quit) {
        mainLoop();
    }

    // clean up
    Config::data.Close();
    delete ip_bar;
    Resources::Destroy();
    main_menu.stop();
    SDL::close();
}
