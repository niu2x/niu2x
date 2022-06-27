#ifndef NX_FFMPEG_MEDIA_H
#define NX_FFMPEG_MEDIA_H

extern "C" {
#include <libavformat/avformat.h>
}

#include <map>

#include <boost/noncopyable.hpp>
#include <niu2x/api.h>

namespace nx::ffmpeg {

class NXAPI media_t : private boost::noncopyable {
public:
    media_t();
    ~media_t();

    int open(const std::string& pathname);
    void close();

    double duration() const;
    double start_time() const;
    int64_t bit_rate() const;

    using metadata_t = std::map<std::string, std::string>;
    metadata_t metadata() const;

    int read(uint8_t* buffer);

    int width() const { return width_; }

    int height() const { return height_; }

private:
    AVFormatContext* avf_;
    AVCodecContext* avc_;
    AVStream* video_stream_;
    int video_stream_idx_;
    int width_, height_;
    uint8_t* video_dst_data_[4];
    int video_dst_linesize_[4];
    int video_dst_bufsize_;
    AVPixelFormat pix_fmt_;

    AVFrame* frame_;
    AVPacket* pkt_;
    bool pkt_valid_;
    int video_frame_count_;

    int open_codec_context(int* stream_idx, AVCodecContext** dec_ctx,
        AVFormatContext* fmt_ctx, enum AVMediaType type);

    int decode_rgb(uint8_t* buffer, AVFrame* frame);
};

} // namespace nx::ffmpeg

#endif