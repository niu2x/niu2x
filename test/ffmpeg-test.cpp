#include <atomic>
#include <set>
#include <mutex>
#include <boost/preprocessor.hpp>
#include <niu2x/ffmpeg.h>
#include "gtest/gtest.h"

TEST(ffmpeg, media)
{
    nx::ffmpeg::media_t media;
    EXPECT_EQ(0, media.open(DIR "gizmo.mp4"));

    EXPECT_NEAR(5.59, media.duration(), 0.01);
    EXPECT_NEAR(0, media.start_time(), 0.01);
    EXPECT_EQ(651605, media.bit_rate());

    auto metadata = media.metadata();
    EXPECT_EQ(metadata["compatible_brands"], "mp42isomavc1");
    EXPECT_EQ(metadata["creation_time"], "2014-03-26T00:20:53.000000Z");
    EXPECT_EQ(metadata["encoder"], "HandBrake 0.9.9 2013052900");
    EXPECT_EQ(metadata["major_brand"], "mp42");
    EXPECT_EQ(metadata["minor_version"], "0");
    media.close();
}