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

private:
    AVFormatContext* avf_;
};

} // namespace nx::ffmpeg

#endif