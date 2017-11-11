// #include <algorithm>

#include "coverflow.h"
#include "SDL.h"
#include "../config/config.h"
#include "../resources/resources.h"
#include "../sdl/sdl.h"
#include "../utils/stringtools.h"
#include "../widgets/animations/blink.h"
#include "../widgets/icon.h"

//#define ICONS_Y 128
#define MARKER_HEIGHT 42

#define SCALE_FACTOR 0.6f

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Coverflow::Coverflow(SDLWindow *wnd) : SelectionTheme(wnd) {
    launch_pending = false;
    last_video_index = 0;

    scrollbar = NULL;
    video = NULL;

    left_offscreen_index = left_index = center_index = right_index = right_offscreen_index = marker_index = 0;

    int ICONS_Y = (StringTools::toInt(Config::data.screen_height)) * 0.12f;

    int CENTER_ICON_WIDTH = (StringTools::toInt(Config::data.screen_width)) * 0.40f;
    int CENTER_ICON_HEIGHT = (StringTools::toInt(Config::data.screen_height)) * 0.40f;

    icon_selected_position.x = (StringTools::toInt(Config::data.screen_width) / 2) - (CENTER_ICON_WIDTH / 2);
    icon_selected_position.y = ICONS_Y;
    icon_selected_position.w = CENTER_ICON_WIDTH;
    icon_selected_position.h = CENTER_ICON_HEIGHT;

    int SMALL_ICON_WIDTH = CENTER_ICON_WIDTH * SCALE_FACTOR;
    int SMALL_ICON_HEIGHT = CENTER_ICON_HEIGHT * SCALE_FACTOR;

    icon_left_position.x = (icon_selected_position.x + CENTER_ICON_WIDTH / 2) - (StringTools::toInt(Config::data.screen_width) / 3) - SMALL_ICON_WIDTH / 2;
    icon_left_position.y = ICONS_Y;
    icon_left_position.w = SMALL_ICON_WIDTH;
    icon_left_position.h = SMALL_ICON_HEIGHT;

    icon_right_position.x = (icon_selected_position.x + CENTER_ICON_WIDTH / 2) + (StringTools::toInt(Config::data.screen_width) / 3) - SMALL_ICON_WIDTH / 2;
    icon_right_position.y = ICONS_Y;
    icon_right_position.w = SMALL_ICON_WIDTH;
    icon_right_position.h = SMALL_ICON_HEIGHT;

    icon_right_offscreen_position.x = -SMALL_ICON_WIDTH;
    icon_right_offscreen_position.y = ICONS_Y;
    icon_right_offscreen_position.w = SMALL_ICON_WIDTH;
    icon_right_offscreen_position.h = SMALL_ICON_HEIGHT;

    icon_left_offscreen_position.w = SMALL_ICON_WIDTH;
    icon_left_offscreen_position.h = SMALL_ICON_HEIGHT;

    MARKER_Y = StringTools::toInt(Config::data.screen_height) - MARKER_HEIGHT;

    gap = icon_selected_position.x - icon_left_position.x;

    addAnimation(&blink_animation);
    addAnimation(&slide_left);
    addAnimation(&slide_center);
    addAnimation(&slide_right);
    addAnimation(&slide_offscreen);

    addAnimation(&scale_left);
    addAnimation(&scale_center);
    addAnimation(&scale_right);
    addAnimation(&scale_offscreen);

    addAnimation(&expand);

    waiting_for_animation = false;

    pending_offset = 0;
    pending_right = false;
    pending_left = false;

    marker = new Icon(window, Config::data.coverflow_scroll_icon);
    createScrollbar(0, MARKER_Y, StringTools::toInt(Config::data.screen_width), MARKER_HEIGHT);

    configureVideo();
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
Coverflow::~Coverflow() {
    delete marker;
    delete video;

    if(scrollbar) {
        SDL_DestroyTexture(scrollbar);
        scrollbar = NULL;
    }
}

// --------------------------------------------------
// onLeft
// --------------------------------------------------
void Coverflow::onLeft() {
    if(center_index - 1 + pending_offset >= 0) {
        if(SDL::SOUND_INITED) {
            Mix_PlayChannel(-1, snd_move, 0);
        }

        shiftLeft();
    }
}

// --------------------------------------------------
// onRight
// --------------------------------------------------
void Coverflow::onRight() {
    if(center_index + 1 + pending_offset < (int)games.size()) {
        if(SDL::SOUND_INITED) {
            Mix_PlayChannel(-1, snd_move, 0);
        }

        shiftRight();
    }
}

// --------------------------------------------------
// onButton
// --------------------------------------------------
void Coverflow::onButton() {
    if(SDL::SOUND_INITED) {
        Mix_PlayChannel(-1, snd_select, 0);
    }

    launch_pending = true;

    SDL_Rect target;

    target.x = 0;
    target.y = 0;
    target.w = StringTools::toInt(Config::data.screen_width);
    target.h = StringTools::toInt(Config::data.screen_height);

    expand.init(icon_selected_position, target, 10);
    expand.changeTarget(games[getSelectionIndex()]->icon);
    expand.start();

    for(unsigned int i = 0; i < games.size(); i++) {
        if((int)i == getSelectionIndex()) {
            continue;
        }

        games[i]->icon->hide();
    }
}

// --------------------------------------------------
// render
// --------------------------------------------------
void Coverflow::render() {
    if(scrollbar) {
        renderScrollbar();
    }

    if(video) {
        video->render();
    }

    if(left_index < center_index) {
        if(games[left_index]->icon) {
            games[left_index]->icon->render();
        }
    }

    if(right_index > center_index) {
        if(games[right_index]->icon) {
            games[right_index]->icon->render();
        }
    }

    if(games[center_index]->icon) {
        games[center_index]->icon->render();
    }

    if(waiting_for_animation) {

        if(right_offscreen_index > right_index &&
                right_offscreen_index > center_index)
            if(games[right_offscreen_index]->icon) {
                games[right_offscreen_index]->icon->render();
            }

        if(left_offscreen_index < left_index &&
                left_offscreen_index < center_index)
            if(games[left_offscreen_index]->icon) {
                games[left_offscreen_index]->icon->render();
            }
    }

    if(marker) {
        marker->render(marker_positions[marker_index]);
    }
}

// --------------------------------------------------
// update
// --------------------------------------------------
void Coverflow::update() {
    bool animations_finished = true;

    for(unsigned int i = 0; i < animations.size(); ++i) {

        if(animations[i]->running) {
            animations[i]->update();

            if(animations[i]->blocking)
                animations_finished = false;
        }
    }

    if(waiting_for_animation && animations_finished) {
        waiting_for_animation = false;

        center_index += pending_offset;
        pending_offset = 0;

        if(center_index < 0) {
            center_index = games.size() - 1;

        } else if(center_index >= (int)games.size()) {
            center_index = 0;
        }

        calcIconIndexes();

        blink_animation.changeTarget(games[center_index]->icon);

        updateVideo();
    }

    if(pending_right) {
        shiftRight();
        pending_right = false;
    }

    if(pending_left) {
        shiftLeft();
        pending_left = false;
    }

    if(animations_finished && launch_pending) {
        launch_pending = false;
        launchGame();
    }
}

// --------------------------------------------------
// addGame
// --------------------------------------------------
void Coverflow::addGame(GAME *new_game) {
    marker_index = center_index = 0;

    games.push_back(new_game);

    // default
    new_game->icon->scale(icon_right_position.w, icon_right_position.h);
    new_game->icon->setPosition(icon_right_offscreen_position.x, icon_highlight_position.y);

    if(left_index < center_index) {
        games[left_index]->icon->setPosition(icon_left_position.x, icon_left_position.y);
    }

    if(right_index > center_index) {
        games[right_index]->icon->setPosition(icon_right_position.x, icon_right_position.y);
    }

    games[center_index]->icon->setPosition(icon_selected_position.x, icon_selected_position.y);

    if(left_index < center_index) {
        games[left_index]->icon->scale(icon_left_position.w, icon_left_position.h);
    }

    if(right_index > center_index) {
        games[right_index]->icon->scale(icon_right_position.w, icon_right_position.h);
    }

    games[center_index]->icon->scale(icon_selected_position.w, icon_selected_position.h);

    if(left_offscreen_index < center_index && left_offscreen_index < left_index) {

        games[left_offscreen_index]->icon->setPosition(0 - games[left_offscreen_index]->icon->getWidth(), icon_left_offscreen_position.y);
        games[left_offscreen_index]->icon->scale(icon_left_offscreen_position.w, icon_left_offscreen_position.h);
    }

    if(right_offscreen_index > center_index && right_offscreen_index > right_index) {

        games[right_offscreen_index]->icon->setPosition(StringTools::toInt(Config::data.screen_width), icon_left_position.y);
        games[right_offscreen_index]->icon->scale(icon_right_offscreen_position.w, icon_right_offscreen_position.h);
    }

    blink_animation.changeTarget(games[center_index]->icon);

    if(video) {
        video->setStreamSource(new_game->video_name);
    }

    calcIconIndexes();
    updateVideo(true);
    calcMarkerDimensions();
}

// --------------------------------------------------
// shiftRight
// --------------------------------------------------
void Coverflow::shiftRight() {
    if(waiting_for_animation) {
        for(unsigned int i = 0; i < animations.size(); ++i) {
            if(animations[i]->running) {
                animations[i]->snap();
            }
        }

        pending_right = true;
        return;
    }

    markerRight();

    // offscreen right to right
    if(right_offscreen_index > center_index && right_offscreen_index > right_index) {

        games[right_offscreen_index]->icon->setPosition(icon_right_position.x + gap, icon_right_position.y);
        slide_offscreen.init(SLIDE_LEFT, icon_right_position.x, 20);
        slide_offscreen.changeTarget(games[right_offscreen_index]->icon);
    }

    // right to center
    if(right_index > center_index) {

        slide_right.init(SLIDE_LEFT, icon_selected_position.x, 30);
        slide_right.changeTarget(games[right_index]->icon);
        scale_right.init(SCALE_UP, icon_right_position, icon_selected_position, 1);
        scale_right.changeTarget(games[right_index]->icon);
    }

    // center to left
    slide_center.init(SLIDE_LEFT, icon_left_position.x, 30);
    slide_center.changeTarget(games[center_index]->icon);

    scale_center.init(SCALE_DOWN, icon_selected_position, icon_left_position, 1);
    scale_center.changeTarget(games[center_index]->icon);

    // left to offscreen
    if(left_index < center_index) {

        slide_left.init(SLIDE_LEFT, 0 - games[left_index]->icon->getWidth(), 20);
        slide_left.changeTarget(games[left_index]->icon);
    }

    blink_animation.stop();
    waiting_for_animation = true;
    pending_offset = 1;
}

// --------------------------------------------------
// shiftLeft
// --------------------------------------------------
void Coverflow::shiftLeft() {
    if(waiting_for_animation) {
        for(unsigned int i = 0; i < animations.size(); ++i) {
            if(animations[i]->running) {
                animations[i]->snap();
            }
        }

        pending_left = true;
        return;
    }

    markerLeft();

    // offscreen left to left
    if(left_offscreen_index < center_index && left_offscreen_index < left_index) {

        games[left_offscreen_index]->icon->setPosition(0 - gap, icon_left_position.y);
        slide_offscreen.init(SLIDE_RIGHT, icon_left_position.x, 20);
        slide_offscreen.changeTarget(games[left_offscreen_index]->icon);
    }

    // left to center
    if(left_index < center_index) {

        slide_left.init(SLIDE_RIGHT, icon_selected_position.x, 30);
        slide_left.changeTarget(games[left_index]->icon);
        scale_left.init(SCALE_UP, icon_left_position, icon_selected_position, 1);
        scale_left.changeTarget(games[left_index]->icon);
    }

    // center to right
    slide_center.init(SLIDE_RIGHT, icon_right_position.x, 30);
    slide_center.changeTarget(games[center_index]->icon);

    scale_center.init(SCALE_DOWN, icon_selected_position, icon_left_position, 1);
    scale_center.changeTarget(games[center_index]->icon);

    // right to offscreen
    if(right_index > center_index) {

        slide_right.init(SLIDE_RIGHT, StringTools::toInt(Config::data.screen_width), 20);
        slide_right.changeTarget(games[right_index]->icon);
    }

    blink_animation.stop();

    waiting_for_animation = true;
    pending_offset = -1;
}

// --------------------------------------------------
// addAnimation
// --------------------------------------------------
void Coverflow::addAnimation(Animation *anim) {
    animations.push_back(anim);
}

// --------------------------------------------------
// calcIconIndexes
// --------------------------------------------------
void Coverflow::calcIconIndexes() {
    left_index = center_index;

    if(--left_index < 0) {
        left_index = games.size() - 1;
    }

    right_index = center_index;

    if(++right_index >= (int)games.size()) {
        right_index = 0;
    }

    left_offscreen_index = center_index;

    // left offscreen
    if(--left_offscreen_index < 0) {
        left_offscreen_index = games.size() - 1;
    }

    if(--left_offscreen_index < 0) {
        left_offscreen_index = games.size() - 1;
    }

    right_offscreen_index = center_index;

    if(++right_offscreen_index >= (int)games.size()) {
        right_offscreen_index = 0;
    }

    if(++right_offscreen_index >= (int)games.size()) {
        right_offscreen_index = 0;
    }
}

// --------------------------------------------------
// calcMarkerDimensions
// --------------------------------------------------
void Coverflow::calcMarkerDimensions() {
    int total_width = StringTools::toInt(Config::data.screen_width);
    int width_per_game = total_width / games.size();

    marker_positions.clear();

    for(unsigned int i = 0; i < games.size(); i++) {

        SDL_Rect r;
        r.x = (i * width_per_game);
        r.y = MARKER_Y;
        r.w = width_per_game;
        r.h = MARKER_HEIGHT;

        marker_positions.push_back(r);
    }
}

// --------------------------------------------------
// markerRight
// --------------------------------------------------
void Coverflow::markerRight() {
    marker_index++;

    if(marker_index < 0) {
        marker_index = games.size() - 1;

    } else if(marker_index >= (int)games.size()) {
        marker_index = 0;
    }
}

// --------------------------------------------------
// markerLeft
// --------------------------------------------------
void Coverflow::markerLeft() {
    marker_index--;

    if(marker_index < 0) {
        marker_index = games.size() - 1;

    } else if(marker_index >= (int)games.size()) {
        marker_index = 0;
    }
}

// --------------------------------------------------
// getSelectionIndex
// --------------------------------------------------
int Coverflow::getSelectionIndex() {
    int selection = center_index;

    if(waiting_for_animation) {
        selection += pending_offset;

        if(selection >= (int)games.size()) {
            selection = 0;

        } else if(selection < 0) {
            selection = games.size() - 1;
        }
    }

    return selection;
}

// --------------------------------------------------
// launchGame
// --------------------------------------------------
void Coverflow::launchGame() {
    //        Mix_Pause(2);
    if(SDL::SOUND_INITED) {
        Mix_FadeOutChannel(2, 1000);
    }

    std::cout << "Executing: " << games[getSelectionIndex()]->exec_command.c_str() << std::endl;
    int err = system(games[getSelectionIndex()]->exec_command.c_str());

    if(err) {
        std::cout << "system call returned: " << err << std::endl;
    }

    launch_pending = false;
    expand.snap();

    for(unsigned int i = 0; i < games.size(); i++) {

        if((int)i == getSelectionIndex())
            continue;

        games[i]->icon->show();
    }

    returning_from_game = true;

    // Mix_Resume(2);
    if(SDL::SOUND_INITED)
        Mix_FadeInChannel(2, snd_bg, -1, 1000);
}

// --------------------------------------------------
// updateVideo
// --------------------------------------------------
void Coverflow::updateVideo(bool force_change) {
    if(video) {

        if(getSelectionIndex() != last_video_index || force_change) {

            last_video_index = getSelectionIndex();

            if(last_video_index < 0 || last_video_index >= (int)games.size()) {
                video->stop();

            } else {
                video->setStreamSource(games[last_video_index]->video_name);
            }
        }
    }
}

// --------------------------------------------------
// createScrollbar
// --------------------------------------------------
void Coverflow::createScrollbar(int x, int y, int w, int h) {
    scrollbar_position.x = x;
    scrollbar_position.y = y;
    scrollbar_position.w = w;
    scrollbar_position.h = h;

    scrollbar_rect.x = 0;
    scrollbar_rect.y = 0;
    scrollbar_rect.w = w;
    scrollbar_rect.h = h;

    if(scrollbar) {
        SDL_DestroyTexture(scrollbar);
        scrollbar = NULL;
    }

    scrollbar = SDL_CreateTexture(
                    window->renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, // SDL_TEXTUREACCESS_STREAMING,
                    scrollbar_position.w, scrollbar_position.h);

    if(scrollbar) {
        SDL_SetTextureBlendMode(scrollbar, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 140);
        SDL_SetRenderTarget(window->renderer, scrollbar);
        SDL_RenderClear(window->renderer);
        SDL_SetRenderTarget(window->renderer, NULL);
    }else{
        std::cout << SDL_GetError() << std::endl;
    }
}

// --------------------------------------------------
// renderScrollbar
// --------------------------------------------------
void Coverflow::renderScrollbar() {
    if(scrollbar) {
        if(SDL_RenderCopy(window->renderer, scrollbar, &scrollbar_rect, &scrollbar_position) != 0) {
            std::cout << SDL_GetError() << std::endl;
        }
    }
}

// --------------------------------------------------
// configureVideo
// --------------------------------------------------
void Coverflow::configureVideo() {
    if(Config::data.video == "disabled") {
        video = NULL; // redundant but jsut a reminder
        return;
    }

    video = new Video(window, true);

    video_position.y = (icon_selected_position.y + icon_selected_position.h) + (StringTools::toInt(Config::data.screen_height) * 0.02f);
    video_position.w = icon_selected_position.w * 1.1f;
    video_position.x = StringTools::toInt(Config::data.screen_width) / 2 - video_position.w / 2;
    video_position.h = StringTools::toInt(Config::data.screen_height) - MARKER_HEIGHT - video_position.y - (StringTools::toInt(Config::data.screen_height) * 0.02f);
    video->setPosition(video_position.x, video_position.y);
    video->scale(video_position.w, video_position.h);
}
