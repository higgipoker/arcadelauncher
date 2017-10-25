#include "staticgrid.h"
#include "../config/config.h"
#include "../resources/resources.h"
#include "../sdl/sdl.h"
#include "../utils/stringtools.h"
#include "../widgets/animations/slide.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BORDER_PERCENT 0.1

#define GRID_COLS StringTools::toInt(Config::data.grid_theme_cols)
#define GRID_ROWS StringTools::toInt(Config::data.grid_theme_rows)

#define SCREEN_WIDTH StringTools::toInt(Config::data.screen_width)
#define SCREEN_HEIGHT StringTools::toInt(Config::data.screen_height)

// --------------------------------------------------
// Constructor
// --------------------------------------------------
StaticGrid::StaticGrid(SDLWindow *wnd) : SelectionTheme(wnd) {
    selection_index = 0;
    animations_finished = true;
    pending_grid_left = pending_grid_right = false;
    preview_video = NULL;
    last_video_index = 0;
    launch_pending = false;
    marker_left = marker_right = NULL;
    current_page = 1;

    if (SDL::SOUND_INITED)
        snd_tick = Mix_LoadWAV("sounds/tick.wav");
    if (snd_tick == NULL) {
        std::cout << "Failed to load sound effect: [sounds/tick.wav], "
                     "SDL_mixer Error:"
                  << Mix_GetError() << std::endl;
    }
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
StaticGrid::~StaticGrid() {
    delete preview_video;
    if (SDL::SOUND_INITED)
        Mix_FreeChunk(snd_tick);
}

// --------------------------------------------------
// init
// --------------------------------------------------
void StaticGrid::init() {
    grids.init(SCREEN_WIDTH, SCREEN_HEIGHT, GRID_ROWS, GRID_COLS);
    createLeftMarker();
    createRightMarker();
    configureVideo();
}

// --------------------------------------------------
// render
// --------------------------------------------------
void StaticGrid::render() {
    // all icons
    for (unsigned int i = 0; i < games.size(); i++) {

        // check if the icon is visible
        if (iconVisible(i)) {

            games[i]->icon->render();
        }
    }
    if (animations_finished)
        if (preview_video)
            preview_video->render();

    if (current_page > 1) {
        if (SDL_RenderCopy(window->renderer, marker_left, &scrollbar_rect_left,
                           &scrollbar_position_left) != 0) {
            std::cout << SDL_GetError() << std::endl;
        }
    }
    if (current_page < grids.numberGrids()) {
        if (SDL_RenderCopy(window->renderer, marker_right,
                           &scrollbar_rect_right,
                           &scrollbar_position_right) != 0) {
            std::cout << SDL_GetError() << std::endl;
        }
    }
}

// --------------------------------------------
// addGame
// --------------------------------------------
void StaticGrid::addGame(GAME *new_game) {
    // add one to grid manager
    grids.addGame();

    // get position for new icon
    SDL_Rect rect = grids.getIconPosition(games.size());

    // TODO: config for borders
    Icon *icon = new_game->icon;
    icon->setPosition(rect.x + 10, rect.y + 10);
    icon->scale(rect.w - 20, rect.h - 20);

    // add to icon list
    games.push_back(new_game);

    // apply the active animation to the latest icon
    blink_animation.changeTarget(games[0]->icon);
    blink_animation.start();
}

// --------------------------------------------------
// getSelectionIndex
// --------------------------------------------------
int StaticGrid::getSelectionIndex() { return selection_index; }

// --------------------------------------------------
// update
// --------------------------------------------------
void StaticGrid::update() {
    // selected icon animation
    blink_animation.update();

    // this animation is only running when a game has been selected
    expand.update();

    // all anims have finished
    animations_finished = true;

    // if at least one anim is running
    int on_finish_anims = animations.size();

    // update all animations
    for (unsigned int i = 0; i < animations.size(); ++i) {

        animations[i]->update();

        if (animations[i]->running) {
            animations_finished = false;
        }
    }

    // an animation set has just finished (shift left.right)
    if (animations_finished && on_finish_anims) {

        // delete tha dynamic animations
        for (unsigned int i = 0; i < animations.size(); i++) {
            delete animations[i];
        }
        animations.clear();

        // conplete (snap in) the animation
        if (pending_grid_left) {
            completeGridLeft();
        }
        if (pending_grid_right) {
            completeGridRight();
        }
    }

    if (selection_index != last_video_index) {
        updateVideo();
    }
    if (!expand.running && launch_pending) {
        launch_pending = false;
        launchGame();
    }
}

// --------------------------------------------------
// onLeft
// --------------------------------------------------
void StaticGrid::onLeft() {
    // suspend during animations
    if (!animations_finished)
        return;

    // decrement selected game
    int tmp = selection_index - 1;

    // work out if we go to the previous screen
    if ((grids.toBaseIndex(tmp) + 1) % GRID_COLS != 0) {

        if (SDL::SOUND_INITED)
            Mix_PlayChannel(-1, snd_tick, 0);
        selection_index = tmp;
        blink_animation.changeTarget(games[selection_index]->icon);

    } else {
        // shift right
        if (grids.onShiftLeft()) {

            if (SDL::SOUND_INITED)
                Mix_PlayChannel(-1, snd_tick, 0);
            // Mix_PlayChannel( -1, snd_move, 0 );
            animateGridsRight();
            current_page--;
        }
    }
}

// --------------------------------------------------
// onRight
// --------------------------------------------------
void StaticGrid::onRight() {
    // suspend during animations
    if (!animations_finished)
        return;

    // increment current selected game
    int tmp = selection_index + 1;

    // work out if we need to go to the next screen
    if ((grids.toBaseIndex(tmp)) % GRID_COLS != 0) {

        if (tmp < (int)games.size()) {

            if (SDL::SOUND_INITED)
                Mix_PlayChannel(-1, snd_tick, 0);
            selection_index = tmp;
            blink_animation.changeTarget(games[selection_index]->icon);
        }
    } else {

        // shift right
        if (grids.onShiftRight()) {
            if (SDL::SOUND_INITED)
                Mix_PlayChannel(-1, snd_tick, 0);

            animateGridsLeft();
            current_page++;
        }
    }
}

// --------------------------------------------------
// onUp
// --------------------------------------------------
void StaticGrid::onUp() {
    // suspend during animations
    if (!animations_finished)
        return;

    int tmp = selection_index - GRID_COLS;

    if (grids.getGrid(tmp) == grids.displayed_grid) {

        if (grids.toBaseIndex(tmp) >= 0) {

            if (SDL::SOUND_INITED)
                Mix_PlayChannel(-1, snd_tick, 0);
            selection_index = tmp;
            blink_animation.changeTarget(games[selection_index]->icon);
        }
    }
}

// --------------------------------------------------
// onDown
// --------------------------------------------------
void StaticGrid::onDown() {
    // suspend during animations
    if (!animations_finished)
        return;

    int tmp = selection_index + GRID_COLS;

    if (grids.getGrid(tmp) == grids.displayed_grid) {

        if (grids.toBaseIndex(tmp) < GRID_ROWS * GRID_COLS) {

            if (tmp < (int)games.size()) {

                if (SDL::SOUND_INITED)
                    Mix_PlayChannel(-1, snd_tick, 0);
                selection_index = tmp;
                blink_animation.changeTarget(games[selection_index]->icon);
            }
        }
    }
}

// --------------------------------------------------
// onButton
// --------------------------------------------------
void StaticGrid::onButton() {
    // suspend during animations
    if (!animations_finished)
        return;

    if (SDL::SOUND_INITED)
        Mix_PlayChannel(-1, snd_select, 0);
    launch_pending = true;
    returning_from_game = true;

    SDL_Rect target;

    target.x = 0;
    target.y = 0;
    target.w = StringTools::toInt(Config::data.screen_width);
    target.h = StringTools::toInt(Config::data.screen_height);

    SDL_Rect initial;
    initial.x = games[selection_index]->icon->getPosition().x;
    initial.y = games[selection_index]->icon->getPosition().y;
    initial.w = games[selection_index]->icon->getWidth();
    initial.h = games[selection_index]->icon->getHeight();
    expand.init(initial, target, 10);
    expand.changeTarget(games[getSelectionIndex()]->icon);
    expand.start();

    if (preview_video) {
        preview_video->hide();
    }

    for (unsigned int i = 0; i < games.size(); i++) {

        if ((int)i == selection_index)
            continue;

        games[i]->icon->hide();
    }
}

// --------------------------------------------------
// launchGame
// --------------------------------------------------
void StaticGrid::launchGame() {
    // Mix_Pause(2);
    if (SDL::SOUND_INITED)
        Mix_FadeOutChannel(2, 1000);

    // Icon* i = (Icon*) games[selection_index]->icon;
    std::cout << games[selection_index]->exec_command.c_str() << std::endl;
    int err = system(games[selection_index]->exec_command.c_str());

    if(err){
        std::cout << "system call returned: " << err << std::endl;
    }

    //         pid_t pid = fork();
    //         if (pid > 0) {
    //                 // We're the parent, so wait for child to finish
    //                 int status;
    //                 waitpid(pid, &status, 0);
    //                 //return status;
    //         }
    //        else if (pid == 0) {
    //                 // We're the child, so run the specified program.  Our
    //                 exit status will
    //                 // be that of the child program unless the execv()
    //                 syscall fails.
    //                 char* argv[1] = {"/home/higgi/Downloads/mame -inipath
    //                 /etc/mame/ wwfwfest"} ;
    //
    //                 std::cout << "LAUNCHING..." << argv[0] << std::endl;
    //                 //std::cout << "LAUNCHING..." << argv[1] << std::endl;
    //
    //                 execv(argv[0],argv);
    //        }

    for (unsigned int i = 0; i < games.size(); i++) {

        if ((int)i == getSelectionIndex())
            continue;

        games[i]->icon->show();
    }
    expand.snap();
    if (preview_video) {
        preview_video->show();
    }
    // Mix_Resume(2);
    if (SDL::SOUND_INITED)
        Mix_FadeInChannel(2, snd_bg, -1, 1000);
}

// --------------------------------------------------
// animateGridsLeft
// --------------------------------------------------
void StaticGrid::animateGridsLeft() {
    pending_grid_left = true;
    animations_finished = false;

    for (unsigned int i = 0; i < games.size(); ++i) {

        if (grids.getGrid(i) == grids.displayed_grid ||
            grids.getGrid(i) == grids.displayed_grid + 1) {

            if (grids.getGrid(i) == grids.displayed_grid + 1) {

                games[i]->icon->move(SCREEN_WIDTH, 0);
            }

            Slide *s = new Slide();
            s->init(SLIDE_LEFT, games[i]->icon->getPosition().x - SCREEN_WIDTH,
                    100);
            s->changeTarget(games[i]->icon);

            animations.push_back(s);
        }
    }
}

// --------------------------------------------------
// animateGridsRight
// --------------------------------------------------
void StaticGrid::animateGridsRight() {
    pending_grid_right = true;
    animations_finished = false;

    for (unsigned int i = 0; i < games.size(); ++i) {

        if (grids.getGrid(i) == grids.displayed_grid ||
            grids.getGrid(i) == grids.displayed_grid - 1) {

            if (grids.getGrid(i) == grids.displayed_grid - 1) {

                games[i]->icon->move(-SCREEN_WIDTH, 0);
            }

            Slide *s = new Slide();
            s->init(SLIDE_RIGHT, games[i]->icon->getPosition().x + SCREEN_WIDTH,
                    100);
            s->changeTarget(games[i]->icon);

            animations.push_back(s);
        }
    }
}

// --------------------------------------------------
// resetPositions
// --------------------------------------------------
void StaticGrid::resetPositions() {
    for (unsigned int i = 0; i < games.size(); i++) {
        SDL_Rect rect = grids.getIconPosition(i);
        games[i]->icon->setPosition(rect.x + 10, rect.y + 10);
    }
}

// --------------------------------------------------
// completeGridLeft
// --------------------------------------------------
void StaticGrid::completeGridLeft() {
    resetPositions();

    grids.displayed_grid++;

    selection_index = grids.plusGrid(selection_index);

    if (selection_index >= (int)games.size()) {
        selection_index = grids.firstIndex();
    }

    blink_animation.changeTarget(games[selection_index]->icon);
    blink_animation.start();
    pending_grid_left = false;
}

// --------------------------------------------------
// completeGridRight
// --------------------------------------------------
void StaticGrid::completeGridRight() {
    resetPositions();

    grids.displayed_grid--;

    selection_index = grids.minusGrid(selection_index);

    blink_animation.changeTarget(games[selection_index]->icon);
    blink_animation.start();
    pending_grid_right = false;
}

// --------------------------------------------------
// iconVisible
// --------------------------------------------------
bool StaticGrid::iconVisible(int index) {
    // figure out which icons to render
    bool icon_visible = false;

    // icon not in screen
    if (games[index]->icon->getPosition().x + games[index]->icon->getWidth() < 0) {
        return false;
    }

    if (games[index]->icon->getPosition().x > SCREEN_WIDTH) {
        return false;
    }

    if (games[index]->icon->getPosition().y + games[index]->icon->getHeight() < 0) {
        return false;
    }

    if (games[index]->icon->getPosition().y > SCREEN_HEIGHT) {
        return false;
    }

    // if this icon is in the currently displayed grid
    if (grids.getGrid(index) == grids.displayed_grid) {
        icon_visible = true;
    }

    // an animation -> left is running so multiple grids are displayed
    if (pending_grid_left) {
        if (!animations_finished &&
            grids.getGrid(index) == grids.displayed_grid + 1) {
            icon_visible = true;
        }
    }

    // an animation -> right is running so multiple grids are displayed
    if (pending_grid_right) {
        if (!animations_finished &&
            grids.getGrid(index) == grids.displayed_grid - 1) {

            icon_visible = true;
        }
    }

    return icon_visible;
}

// --------------------------------------------------
// updateVideo
// --------------------------------------------------
void StaticGrid::updateVideo() {
    if (preview_video) {

        if (selection_index != last_video_index) {

            last_video_index = selection_index;

            if (last_video_index < 0 || last_video_index >= (int)games.size()) {
                preview_video->stop();
            } else {
                preview_video->setStreamSource(games[selection_index]->video_name);
                preview_video->setPosition(games[selection_index]->icon->getPosition().x + 4, games[selection_index]->icon->getPosition().y + 4);
                preview_video->scale(games[selection_index]->icon->getWidth() - 8, games[selection_index]->icon->getHeight() - 8);
            }
        }
    }
}

#define MARKER_WIDTH 30
// --------------------------------------------------
// createLeftMarker
// --------------------------------------------------
void StaticGrid::createLeftMarker() {
    Icon *arrow_left = new Icon(window, "gfx/arrow_left.png");

    scrollbar_rect_left.x = 0;
    scrollbar_rect_left.y = 0;
    scrollbar_rect_left.w = MARKER_WIDTH;
    scrollbar_rect_left.h = SCREEN_HEIGHT;

    scrollbar_position_left.x = 0;
    scrollbar_position_left.y = 0;
    scrollbar_position_left.w = MARKER_WIDTH;
    scrollbar_position_left.h = SCREEN_HEIGHT;

    arrow_left->setPosition(0, (SCREEN_HEIGHT / 2) - arrow_left->getHeight() / 2);
    arrow_left->scale(MARKER_WIDTH, MARKER_WIDTH);

    if (marker_left) {
        SDL_DestroyTexture(marker_left);
        marker_left = NULL;
    }

    marker_left = SDL_CreateTexture(window->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, /* SDL_TEXTUREACCESS_STREAMING,*/ scrollbar_position_left.w, scrollbar_position_left.h);

    if (!marker_left) {
        std::cout << SDL_GetError() << std::endl;
    }

    if (marker_left) {
        SDL_SetTextureBlendMode(marker_left, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 200);
        SDL_SetRenderTarget(window->renderer, marker_left);
        SDL_RenderClear(window->renderer);
        arrow_left->render();
        SDL_SetRenderTarget(window->renderer, NULL);
    }

    delete arrow_left;
}

// --------------------------------------------------
// createRightMarker
// --------------------------------------------------
void StaticGrid::createRightMarker() {
    Icon *arrow_right = new Icon(window, "gfx/arrow_right.png");

    scrollbar_rect_right.x = 0;
    scrollbar_rect_right.y = 0;
    scrollbar_rect_right.w = MARKER_WIDTH;
    scrollbar_rect_right.h = SCREEN_HEIGHT;

    scrollbar_position_right.x = SCREEN_WIDTH - MARKER_WIDTH;
    scrollbar_position_right.y = 0;
    scrollbar_position_right.w = MARKER_WIDTH;
    scrollbar_position_right.h = SCREEN_HEIGHT;

    arrow_right->setPosition(0, (SCREEN_HEIGHT / 2) - arrow_right->getHeight() / 2);
    arrow_right->scale(MARKER_WIDTH, MARKER_WIDTH);

    if (marker_right) {
        SDL_DestroyTexture(marker_right);
        marker_right = NULL;
    }

    marker_right = SDL_CreateTexture(window->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, /* SDL_TEXTUREACCESS_STREAMING,*/ scrollbar_position_right.w, scrollbar_position_right.h);

    if (!marker_right) {
        std::cout << SDL_GetError() << std::endl;
    }

    if (marker_right) {
        SDL_SetTextureBlendMode(marker_right, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 200);
        SDL_SetRenderTarget(window->renderer, marker_right);
        SDL_RenderClear(window->renderer);
        arrow_right->render();
        SDL_SetRenderTarget(window->renderer, NULL);
    }

    delete arrow_right;
}

// --------------------------------------------------
// configureVideo
// --------------------------------------------------
void StaticGrid::configureVideo() {
    if (Config::data.video == "disabled") {
        preview_video = NULL; // redundant but jsut a reminder
        return;
    }

    preview_video = new Video(window, false);
}
