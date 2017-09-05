#include "video.h"
#include "icon.h"
#include <sys/stat.h>
#include <unistd.h>

#define FRAME_TIMER 30
#define LOAD_TIMER 300
#define STREAM_TIMER 500

// --------------------------------------------------
// Video
// --------------------------------------------------
Video::Video(SDLWindow *wnd, bool with_border) {
    stream = NULL;
    video_texture = NULL;
    window = wnd;
    stream_started = false;

    stream = new VideoStreamer2();

    if (with_border) {
        border = new Icon(wnd, "gfx/video_frame.png");
        border_width = 6;
    } else {
        border = NULL;
        border_width = 0;
    }
}

void Video::setStreamSource(const std::string &filename) {

    if (access(filename.c_str(), F_OK) == -1) {
        stream_started = false;
        return;
    }

    if (video_texture)
        clearScreen();

    delete stream;
    stream = new VideoStreamer2();
    if (stream->start(filename))
        stream_started = true;
    frame_timer.start();

    videoRectSrc.x = 0;
    videoRectSrc.y = 0;
    videoRectSrc.w = stream->getWidth();
    videoRectSrc.h = stream->getHeight();

    createTexture();
}

// --------------------------------------------------
// ~Video
// --------------------------------------------------
Video::~Video() {
    //         if(stream_started)
    //                 stream->stop();
    //         delete stream;

    delete stream;

    if (video_texture) {
        SDL_DestroyTexture(video_texture);
        video_texture = NULL;
    }
    delete border;
}

// --------------------------------------------------
// render
// --------------------------------------------------
void Video::render() {
    if (stream_started) {

        if (frame_timer.get_ticks() >= FRAME_TIMER) {

            AVFrame *pFrame = stream->poll();

            if (pFrame) {

                if (visible) {
                    if (SDL_UpdateTexture(video_texture, &videoRectSrc,
                                          pFrame->data[0],
                                          pFrame->linesize[0]) != 0) {
                        std::cout << SDL_GetError() << std::endl;
                    }
                }

                frame_timer.start();
                // av_freep(pFrame);
            }
        }

        if (visible) {

            if (SDL_RenderCopy(window->renderer, video_texture, &videoRectSrc,
                               &videoRectDst) != 0) {
                // std::cout << SDL_GetError()   << std::endl;
            }
            if (border)
                border->render();
        }
    }
}

// --------------------------------------------------
// setPosition
// --------------------------------------------------
void Video::setPosition(int _x, int _y) {
    videoRectDst.x = _x + border_width;
    videoRectDst.y = _y + border_width;

    if (border)
        border->setPosition(_x, _y);
}

// --------------------------------------------------
// blend
// --------------------------------------------------
void Video::blend(int alpha) {}

// --------------------------------------------------
// move
// --------------------------------------------------
void Video::move(int _x, int _y) {}

// --------------------------------------------------
// getWidth
// --------------------------------------------------
int Video::getWidth() { return videoRectDst.w; }

// --------------------------------------------------
// getHeight
// --------------------------------------------------
int Video::getHeight() { return videoRectDst.h; }

// --------------------------------------------------
// stop
// --------------------------------------------------
void Video::stop() {
    //         if(stream_started){
    //                 stream->stop();
    //                 stream_started = false;
    //         }

    if (stream) {
        delete stream;
        stream = NULL;
    }

    if (video_texture)
        SDL_DestroyTexture(video_texture);

    video_texture = NULL;
}

// --------------------------------------------------
// clearScreen
// --------------------------------------------------
void Video::clearScreen() {
    if (video_texture) {
        SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
        SDL_SetRenderTarget(window->renderer, video_texture);
        SDL_RenderClear(window->renderer);
        SDL_SetRenderTarget(window->renderer, NULL);
    }
}

// --------------------------------------------------
// getPosition
// --------------------------------------------------
Point Video::getPosition() { return Point(videoRectDst.x, videoRectDst.y); }

// --------------------------------------------------
// scale
// --------------------------------------------------
void Video::scale(int _w, int _h) {
    if (_w && _h) {
        videoRectSrc.x = 0;
        videoRectSrc.y = 0;
        videoRectSrc.w = stream->getWidth();
        videoRectSrc.h = stream->getHeight();

        videoRectDst.w = _w - (border_width * 2);
        videoRectDst.h = _h - (border_width * 2);

        if (border)
            border->scale(_w, _h);

        if (videoRectSrc.w && videoRectSrc.h) {
            createTexture();
        }
    }
}

void Video::createTexture() {
    if (video_texture) {

        SDL_DestroyTexture(video_texture);
        video_texture = NULL;
    }

    video_texture = SDL_CreateTexture(
        window->renderer,
        SDL_PIXELFORMAT_BGR24,    // SDL_PIXELFORMAT_YV12,
        SDL_TEXTUREACCESS_TARGET, // SDL_TEXTUREACCESS_STREAMING,
        videoRectSrc.w, videoRectSrc.h);
    if (!video_texture) {
        std::cout << SDL_GetError() << std::endl;
    }

    SDL_SetTextureBlendMode(video_texture, SDL_BLENDMODE_BLEND);
    clearScreen();
}
