#include "videostreamer2.h"
#include <iostream>

VideoStreamer2::VideoStreamer2() {
  pFormatCtx = NULL;
  pCodecCtx = NULL;
  pFrame = NULL;
  pFrameRGB = NULL;
  buffer = NULL;
  frameFinished = 0;
  videoStream = -1;
  sws_ctx = NULL;

  started = false;
}

VideoStreamer2::~VideoStreamer2() {
  stop();
}

bool VideoStreamer2::start(const std::string &filename) {
  started = false;

  pFormatCtx = NULL;
  pCodecCtx = NULL;
  pFrame = NULL;
  pFrameRGB = NULL;
  buffer = NULL;
  frameFinished = 0;
  videoStream = -1;

  if (avformat_open_input(&pFormatCtx, filename.c_str(), NULL, NULL) != 0){
      std::cout << "could not open " << filename << std::endl;
    return false; // Couldn't open file
    }

  if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
      std::cout << "could not find a video stream" << std::endl;
    return false; // Couldn't find stream information
  }

  for (unsigned i = 0; i < pFormatCtx->nb_streams; i++) {

    if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStream = i;
      break;
    }
  }

  if (videoStream == -1)
    return false; // Didn't find a video stream

  pCodecCtx = pFormatCtx->streams[videoStream]->codec;

  AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

  if (pCodec == NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return false; // Codec not found
  }

  // Open codec
  AVDictionary *optionsDict = NULL;
  if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0)
    return false; // Could not open codec

  AVPixelFormat dst_fix_fmt = (AVPixelFormat)3; // PIX_FMT_YUV420P

  // Allocate video frame
  sws_ctx =
   sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, dst_fix_fmt, SWS_BILINEAR, NULL, NULL, NULL);

  pFrame = av_frame_alloc();

  pFrameRGB = av_frame_alloc();

  if (pFrameRGB == NULL)
    return false;

  int numBytes = avpicture_get_size(dst_fix_fmt, // PIX_FMT_YUV420P,
                                    pCodecCtx->width, pCodecCtx->height);

  buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

  avpicture_fill((AVPicture *)pFrameRGB, buffer, dst_fix_fmt, pCodecCtx->width, pCodecCtx->height);

  started = true;
  return true;
}

AVFrame *VideoStreamer2::poll() {
  if (!started)
    return NULL;

  AVFrame *return_frame = NULL;
  AVPacket packet;

  if (av_read_frame(pFormatCtx, &packet) >= 0) {

    // Is this a packet from the video stream?
    if (packet.stream_index == videoStream) {

      // Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

      // Did we get a video frame?
      if (frameFinished) {

        sws_scale(sws_ctx, (uint8_t const *const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
        return_frame = pFrameRGB;
      }
    }

    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  } else {
    // loop back to start
    av_seek_frame(pFormatCtx, videoStream, 0, AVSEEK_FLAG_ANY);
  }

  return return_frame;
}

void VideoStreamer2::stop() {
  if (!started)
    return;

  // Free the RGB image
  av_free(buffer);
  av_free(pFrameRGB);

  // Free the YUV frame
  av_free(pFrame);

  // Close the codec
  avcodec_close(pCodecCtx);

  // Close the video file
  avformat_close_input(&pFormatCtx);

  sws_freeContext(sws_ctx);
}

int VideoStreamer2::getWidth() {
  if (pCodecCtx)
    return pCodecCtx->width;

  else
    return 0;
}

int VideoStreamer2::getHeight() {
  if (pCodecCtx)
    return pCodecCtx->height;

  else
    return 0;
}
