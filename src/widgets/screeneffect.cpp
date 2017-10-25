#include "screeneffect.h"
#include "../config/config.h"
#include "../utils/stringtools.h"

ScreenEffect::ScreenEffect(SDLWindow *wnd, const std::string &filename, int width, int height) {
    flicker = false;
    intensity = flicker_intensity = 0;

    intensity = StringTools::toInt(Config::data.screen_effect_intensity);
    flicker_intensity = StringTools::toInt(Config::data.screen_effect_intensity_flicker);

    intensity *= 2.55;
    flicker_intensity *= 2.55;

    window = wnd;

    overlay = SDL_CreateTexture(window->renderer,
                                SDL_PIXELFORMAT_BGR24, // SDL_PIXELFORMAT_YV12,
                                SDL_TEXTUREACCESS_STATIC /*SDL_TEXTUREACCESS_STREAMING*/ | SDL_TEXTUREACCESS_TARGET,
                                width, height);
    if (!overlay) {
        std::cout << SDL_GetError() << std::endl;
    }

    SDL_SetTextureBlendMode(overlay, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(overlay, 20);

    Image *image = new Image(window, filename);

    unsigned int image_width = image->getWidth();
    unsigned int image_height = image->getHeight();

    unsigned int images_per_width = width / image_width;
    unsigned int images_per_height = height / image_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = image_width;
    src_rect.h = image_height;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = image_width;
    dst_rect.h = image_height;

    SDL_SetRenderTarget(window->renderer, overlay);

    for (unsigned int y = 0; y < images_per_height; ++y) {

        dst_rect.y = y * image_height;

        for (unsigned int x = 0; x < images_per_width; ++x) {

            dst_rect.x = x * image_width;
            SDL_RenderCopy(window->renderer, image->tex, &src_rect, &dst_rect);
        }
    }

    SDL_SetRenderTarget(window->renderer, NULL);

    src_rect.w = width;
    src_rect.h = height;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = width;
    dst_rect.h = height;

    delete image;
}
ScreenEffect::~ScreenEffect() {
    SDL_DestroyTexture(overlay);
}

// --------------------------------------------------
// render
// --------------------------------------------------
void ScreenEffect::render() {
    if (flicker_intensity)
        flicker = !flicker;

    if (flicker) {
        SDL_SetTextureAlphaMod(overlay, flicker_intensity);
    } else {
        SDL_SetTextureAlphaMod(overlay, intensity);
    }
    SDL_RenderCopy(window->renderer, overlay, &src_rect, &dst_rect);
}

// --------------------------------------------------
// render
// --------------------------------------------------
bool ScreenEffect::render(const SDL_Rect &target_rect) {
    return false;
}

// --------------------------------------------------
// setPosition
// --------------------------------------------------
void ScreenEffect::setPosition(int _x, int _y) {
}

// --------------------------------------------------
// blend
// --------------------------------------------------
void ScreenEffect::blend(int alpha) {
}

// --------------------------------------------------
// move
// --------------------------------------------------
void ScreenEffect::move(int _x, int _y) {
}

// --------------------------------------------------
// getWidth
// --------------------------------------------------
int ScreenEffect::getWidth() {
    return 0;
}

// --------------------------------------------------
// getHeight
// --------------------------------------------------
int ScreenEffect::getHeight() {
    return 0;
}
