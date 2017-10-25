#include "textbar.h"

static const int TIME_TO_SHOW = 6000;

TextBar::TextBar(SDLWindow *window, const std::string &str, int w, int h):
wnd (*window){

    width = w;
    height = h;
    text = std::make_unique<SDL_Text>(window, "fonts/atari full.ttf", 14);
    text->setText(str);

    SDL_Color c;
    c.r = 0;
    c.g = 0;
    c.b = 0;
    c.a = 255;
    text->setColor(c);
    text->setPosition(12, 12);

    hide();
}

void TextBar::render() {

    if (visible) {
        SDL_SetRenderDrawColor(wnd.renderer, 0, 0, 0, 255);
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = width;
        rect.h = height;
        SDL_RenderFillRect(wnd.renderer, &rect);

        text->render();

        if (timer.get_ticks() > TIME_TO_SHOW) {
            timer.stop();
            hide();
        }
    }
}

void TextBar::setPosition(int _x, int _y) {}

int TextBar::getHeight() { return height; }
int TextBar::getWidth() { return width; }

void TextBar::show() {
    Widget::show();
    timer.start();
}
