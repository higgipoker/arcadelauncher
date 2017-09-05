#include "text.h"
#include <sstream>

SDL_Text::SDL_Text(SDLWindow *window, const std::string &font_name,
                   int font_height, const std::string &text, bool _shadow)
    : Renderable(window) {
    shadow_texture = NULL;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    // an sdl font
    font = TTF_OpenFont(font_name.c_str(), font_height);

    save_position.x = 0;
    save_position.y = 0;

    shadow = _shadow;
    color_shadow.r = 1;
    color_shadow.g = 1;
    color_shadow.b = 1;
    color_shadow.a = 140;

    setText(text);
    setPosition(0, 0);
    hud = false;
}

SDL_Text::SDL_Text(SDLWindow *window, const std::string &font_name,
                   int font_height, bool _shadow)
    : Renderable(window), t("") {
    // an sdl font
    font = TTF_OpenFont(font_name.c_str(), font_height);

    shadow_texture = NULL;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    shadow = _shadow;

    save_position.x = 0;
    save_position.y = 0;

    color_shadow.r = 1;
    color_shadow.g = 1;
    color_shadow.b = 1;
    color_shadow.a = 140;

    setPosition(0, 0);
    hud = false;
}

SDL_Text::~SDL_Text() {
    TTF_CloseFont(font);

    if (shadow_texture)
        SDL_DestroyTexture(shadow_texture);
}

bool SDL_Text::render() {
    if (tex && t != "") {
        SDL_RenderCopy(wnd->renderer, tex, &src_rect, &dst_rect);
        return true;
    }

    return false;
}

void SDL_Text::setText(const std::string &text) {
    t = text;
    if (tex)
        SDL_DestroyTexture(tex);

    SDL_Surface *sfc = TTF_RenderText_Solid(font, text.c_str(), color);
    tex = SDL_CreateTextureFromSurface(wnd->renderer, sfc);

    //  before deleting the temp surface, get the dimensions
    surface_width = sfc->w;
    surface_height = sfc->h;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = sfc->w;
    src_rect.h = sfc->h;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = sfc->w;
    dst_rect.h = sfc->h;

    screen_rect.x = 0;
    screen_rect.y = 0;
    screen_rect.w = sfc->w;
    screen_rect.h = sfc->h;

    SDL_FreeSurface(sfc);

    setPosition(save_position.x, save_position.y);

    if (shadow) {
        if (shadow_texture)
            SDL_DestroyTexture(shadow_texture);

        SDL_Surface *sfc_shadow =
            TTF_RenderText_Solid(font, text.c_str(), color_shadow);
        shadow_texture =
            SDL_CreateTextureFromSurface(wnd->renderer, sfc_shadow);
        SDL_FreeSurface(sfc_shadow);
    }
}

void SDL_Text::setText(int i) {
    std::ostringstream s;
    s << i;
    setText(s.str());
}

//  --------------------------------------------------
//  set position
//  --------------------------------------------------
void SDL_Text::setPosition(int x, int y) {
    dst_rect.x = x;
    dst_rect.y = y;

    save_position.x = x;
    save_position.y = y;

    shadow_rect.x = dst_rect.x + 5;
    shadow_rect.y = dst_rect.y + 5;
    shadow_rect.w = dst_rect.w;
    shadow_rect.h = dst_rect.h;
}
