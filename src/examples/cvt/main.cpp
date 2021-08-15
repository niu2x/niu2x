#include <fstream>

#include <niu2x/io.h>
#include <niu2x/string_utils.h>

#include <common/cxxopts.h>

int main(int argc, char* argv[])
{
    cxxopts::Options options("cvt", "niu2x's cvt tools");

    options.add_options()("o,output", "output", cxxopts::value<std::string>())(
        "i,input", "input", cxxopts::value<std::string>()->default_value(""))(
        "codec", "codec", cxxopts::value<std::string>()->default_value(""))(
        "h,help", "Print usage");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    // std::unique_ptr<nx::io::base_source<uint8_t>> source;
    // std::unique_ptr<nx::io::base_sink<uint8_t>> sink;
    // source =
    //
    auto codec = result["codec"].as<std::string>();

    auto filters = nx::string_utils::split(codec, '|');
    std::vector<uint8_t> buffer[2];

    {
        std::string input = result["input"].as<std::string>();
        if (input == "") {
            nx::io::vector sink(buffer[0]);
            nx::io::pipe(nx::io::cin, nx::io::sink_proxy(&sink));
        } else {
            nx::io::vector sink(buffer[0]);
            std::fstream file(input.c_str());
            nx::io::std_istream_adapter source(file);
            nx::io::pipe(
                nx::io::source_proxy(&source), nx::io::sink_proxy(&sink));
        }
    }

    std::vector<uint8_t>* src_v = &buffer[0];
    std::vector<uint8_t>* sink_v = &buffer[1];

    for (auto& filter_name : filters) {
        nx::io::inbuf src(src_v->data(), src_v->size());
        nx::io::vector sink(*sink_v);

        nx::io::source_proxy source_proxy(&src);
        nx::io::sink_proxy sink_proxy(&sink);

        if (filter_name == "hex_encode") {
            nx::io::pipe(source_proxy, nx::io::hex_encode, sink_proxy);
        } else if (filter_name == "hex_decode") {
            nx::io::pipe(source_proxy, nx::io::hex_decode, sink_proxy);
        } else if (filter_name == "zlib_compress") {
            nx::io::zlib_compress_filter zlib_compress;
            auto filter = nx::io::filter_proxy(&zlib_compress);
            nx::io::pipe(source_proxy, filter, sink_proxy);
        } else if (filter_name == "zlib_uncompress") {
            nx::io::zlib_uncompress_filter zlib_uncompress;
            auto filter = nx::io::filter_proxy(&zlib_uncompress);
            nx::io::pipe(source_proxy, filter, sink_proxy);

        } else if (filter_name == "") {
            continue;
        } else {
            NX_THROW("Unsupported codec " + filter_name);
        }
        std::swap(src_v, sink_v);
        sink_v->clear();
    }

    {
        nx::io::inbuf src(src_v->data(), src_v->size());
        nx::io::pipe(nx::io::source_proxy(&src), nx::io::cout);
    }

    return 0;
}