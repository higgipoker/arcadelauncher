#include "widget.h"

Widget::Widget() {
    lit = false;
    scale_x = scale_y = 1.0f;
    size_w = size_h = 0;
    z_index = 0;
    visible = true;
    fade_direction = 0;
}
Widget::~Widget() {}

void Widget::update() {
    if (fade_direction != 0) {
        // if(timer.get_ticks()>1){
        timer.start();
        alpha += fade_direction * 5;
        if (alpha < 0) {
            fade_direction = 0;
            alpha = 0;
        } else if (alpha > 255) {
            fade_direction = 0;
            alpha = 255;
        }
        //}
    }
}

void Widget::highlight(bool state) { lit = state; }

void Widget::toggleHighlight() { lit = !lit; }

void Widget::scale(float _w, float _h) {
    scale_x = _w;
    scale_y = _h;
}

void Widget::scale(int _w, int _h) {
    size_w = _w;
    size_h = _h;
}

void Widget::show() { visible = true; }

void Widget::hide() { visible = false; }

void Widget::blend(int _alpha) {

    alpha = _alpha;

    if (alpha > 255) {
        alpha = 255;
    }
    if (alpha < 0) {
        alpha = 0;
    }
}

void Widget::fadeOut(void) {
    alpha = 255;
    fade_direction = -1;
    timer.start();
}

void Widget::fadeIn(void) {
    alpha = 0;
    fade_direction = +1;
    timer.start();
}
