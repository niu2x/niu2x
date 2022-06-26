#include <niu2x/ffmpeg/media.h>
#include <niu2x/utils.h>

namespace nx::ffmpeg {

media_t::media_t()
: avf_(nullptr)
{
}
media_t::~media_t() { close(); }

int media_t::open(const std::string& pathname)
{
    NX_FAIL_COND_V(avf_ != nullptr, -E_BUSY);
    int ret = avformat_open_input(&avf_, pathname.c_str(), nullptr, nullptr);
    NX_FAIL_COND_V_MSG(
        ret < 0, -E_FFMPEG_OPEN, "avformat_open_input fail: %d", ret);

    ret = avformat_find_stream_info(avf_, nullptr);
    NX_FAIL_COND_V_MSG(
        ret < 0, -E_FFMPEG_OPEN, "avformat_find_stream_info fail: %d", ret);

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
    NX_FAIL_COND(!avf_);
    avformat_close_input(&avf_);
    avf_ = nullptr;
}

} // namespace nx::ffmpeg