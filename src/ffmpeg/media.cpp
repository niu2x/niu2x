#include <niu2x/ffmpeg/media.h>

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "yuv_rgb.h"
}

#include "stb_image_write.h"

#include <niu2x/utils.h>

namespace nx::ffmpeg {

media_t::media_t()
: avf_(nullptr)
, avc_(nullptr)
, video_stream_(nullptr)
, video_stream_idx_(0)
, width_(0)
, height_(0)
, frame_(nullptr)
, pkt_(nullptr)
, pkt_valid_(false)
, video_frame_count_(0)
{
    memset(video_dst_data_, 0, sizeof(video_dst_data_));
    memset(video_dst_linesize_, 0, sizeof(video_dst_linesize_));
}
media_t::~media_t() { close(); }

int media_t::open_codec_context(int* stream_idx, AVCodecContext** dec_ctx,
    AVFormatContext* fmt_ctx, enum AVMediaType type)
{
    int ret, stream_index;
    AVStream* st;
    const AVCodec* dec = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        NX_LOG_E("Could not find %s stream\n", av_get_media_type_string(type));
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];

        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        if (!dec) {
            NX_LOG_E(
                "Failed to find %s codec\n", av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            NX_LOG_E("Failed to allocate the %s codec context\n",
                av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
            NX_LOG_E("Failed to copy %s codec parameters to decoder context\n",
                av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders */
        if ((ret = avcodec_open2(*dec_ctx, dec, NULL)) < 0) {
            NX_LOG_E(
                "Failed to open %s codec\n", av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }

    return 0;
}

int media_t::open(const std::string& pathname)
{
    NX_FAIL_COND_V(avf_ != nullptr, -E_BUSY);
    int ret = avformat_open_input(&avf_, pathname.c_str(), nullptr, nullptr);
    NX_FAIL_COND_V_MSG(
        ret < 0, -E_FFMPEG_OPEN, "avformat_open_input fail: %d", ret);

    ret = avformat_find_stream_info(avf_, nullptr);
    NX_FAIL_COND_V_MSG(
        ret < 0, -E_FFMPEG_OPEN, "avformat_find_stream_info fail: %d", ret);

    ret = open_codec_context(
        &video_stream_idx_, &avc_, avf_, AVMEDIA_TYPE_VIDEO);

    NX_FAIL_COND_V_MSG(ret < 0, -E_FFMPEG_OPEN, "no video stream");

    video_stream_ = avf_->streams[video_stream_idx_];

    /* allocate image where the decoded image will be put */
    width_ = avc_->width;
    height_ = avc_->height;
    pix_fmt_ = avc_->pix_fmt;

    ret = av_image_alloc(
        video_dst_data_, video_dst_linesize_, width_, height_, pix_fmt_, 1);
    NX_FAIL_COND_V_MSG(
        ret < 0, -E_FFMPEG_OPEN, "Could not allocate raw video buffer");
    video_dst_bufsize_ = ret;

    frame_ = av_frame_alloc();
    if (!frame_) {
        return -E_FFMPEG_OPEN;
    }

    pkt_ = av_packet_alloc();
    if (!pkt_) {
        return -E_FFMPEG_OPEN;
    }

    return E_OK;
}

int64_t media_t::bit_rate() const
{
    NX_FAIL_COND_V(!avf_, E_FFMPEG_NOT_OPEN);
    return avf_->bit_rate;
}

media_t::metadata_t media_t::metadata() const
{
    metadata_t metadata;
    NX_FAIL_COND_V(!avf_, {});

    AVDictionaryEntry* next = nullptr;
    do {
        next = av_dict_get(avf_->metadata, "", next, AV_DICT_IGNORE_SUFFIX);
    } while ((next) && (metadata[next->key] = next->value, true));
    return metadata;
}

double media_t::start_time() const
{
    NX_FAIL_COND_V(!avf_, E_FFMPEG_NOT_OPEN);
    return (double)(avf_->start_time) / AV_TIME_BASE;
}

double media_t::duration() const
{
    NX_FAIL_COND_V(!avf_, E_FFMPEG_NOT_OPEN);
    return (double)(avf_->duration) / AV_TIME_BASE;
}

void media_t::close()
{
    av_frame_free(&frame_);

    if (pkt_) {
        if (pkt_valid_) {
            av_packet_unref(pkt_);
            pkt_valid_ = false;
        }

        av_packet_free(&pkt_);

        pkt_ = nullptr;
    }
    if (avc_) {
        avcodec_free_context(&avc_);
        avc_ = nullptr;
    }
    if (avf_) {
        avformat_close_input(&avf_);
        avf_ = nullptr;
    }

    if (video_dst_data_[0]) {
        av_free(video_dst_data_[0]);
        video_dst_data_[0] = nullptr;
    }
}

int media_t::decode_rgb(uint8_t* buffer, AVFrame* frame)
{
    av_image_copy(video_dst_data_, video_dst_linesize_,
        (const uint8_t**)(frame->data), frame->linesize, pix_fmt_, width_,
        height_);

    unused(buffer, frame);
    switch (pix_fmt_) {
        case AV_PIX_FMT_YUV420P: {
            yuv420_rgb24_std(width_, height_, video_dst_data_[0],
                video_dst_data_[1], video_dst_data_[2], video_dst_linesize_[0],
                video_dst_linesize_[1], buffer, width_ * 3, YCBCR_601);
            // char filename[128];
            // sprintf(filename, "./test-%03d.jpg", k++);
            // stbi_write_jpg(filename, width_, height_, 3, buffer, 100);
            break;
        }
        default: {
            NX_LOG_E("unsupport pix_fmt %d\n", pix_fmt_);
            break;
        }
    }

    return E_OK;
}

int media_t::read(uint8_t* buffer)
{
    NX_FAIL_COND_V(!avf_, -E_OPERATION);
    NX_FAIL_COND_V(!avc_, -E_OPERATION);

    if (!pkt_valid_) {
        int ret = av_read_frame(avf_, pkt_);
        if (ret < 0) {
            pkt_valid_ = false;
        } else {
            pkt_valid_ = true;
        }
        avcodec_send_packet(avc_, pkt_);
    }

    if (pkt_valid_) {
        int ret = avcodec_receive_frame(avc_, frame_);
        if (ret < 0) {
            av_packet_unref(pkt_);
            pkt_valid_ = false;
            return read(buffer);
        } else {
            if (buffer)
                decode_rgb(buffer, frame_);
            av_frame_unref(frame_);
            return E_OK;
        }
    }

    return E_EOF;
}

} // namespace nx::ffmpeg