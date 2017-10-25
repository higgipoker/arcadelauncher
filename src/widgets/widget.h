#pragma once

#include "../utils/point.h"
#include "../utils/timer.h"

/** @brief BaseWidget */
class Widget {
  public:
    Widget();
    virtual void render() = 0;
    virtual void setPosition(int _x, int _y) = 0;
    virtual Point getPosition() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual void update();

    virtual void move(int _x, int _y){}
    virtual void highlight(bool state);
    virtual void toggleHighlight();
    virtual void scale(float _w, float _h);
    virtual void scale(int _w, int _h);
    virtual void blend(int _alpha);

    virtual bool render(const SDL_Rect &target_rect) { return false; }

    void show();
    void hide();
    void fadeOut(void);
    void fadeIn(void);

    bool operator<(const Widget &val) const { return z_index < val.z_index; }

    virtual ~Widget();

  protected:
    bool lit;
    float scale_x;
    float scale_y;
    int size_w;
    int size_h;
    int z_index;
    bool visible;
    int alpha;
    int fade_direction;

    Timer timer;
};
