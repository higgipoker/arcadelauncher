#pragma once

#include <string>

#ifdef __cplusplus
extern "C" {
#endif
#include "libswscale/swscale.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

class VideoStreamer2 {
  public:
    VideoStreamer2();
    ~VideoStreamer2();

    bool start(const std::string &filename);
    AVFrame *poll();

    int getWidth();
    int getHeight();

  private:
    void stop();
    AVFormatContext *pFormatCtx;
    int videoStream;
    AVCodecContext *pCodecCtx;
    AVFrame *pFrame;
    AVFrame *pFrameRGB;
    int frameFinished;
    uint8_t *buffer;
    SwsContext *sws_ctx;

    bool started;
};
