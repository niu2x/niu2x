#include <niu2x/cvt.h>

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

    // std::unique_ptr<nx::cvt::base_source<uint8_t>> source;
    // std::unique_ptr<nx::cvt::base_sink<uint8_t>> sink;
    // source =
    //
    auto& source = nx::cvt::source::cin;
    auto& sink = nx::cvt::sink::cout;

    auto codec = result["codec"].as<std::string>();

    auto filters = nx::string_utils::split(codec, '|');
    std::vector<uint8_t> buffer[2];

    {
        std::string input = result["input"].as<std::string>();
        if (input == "") {
            nx::cvt::sink::adapter<uint8_t, std::vector<uint8_t>> sink(
                buffer[0]);
            nx::cvt::pipe(nx::cvt::source::cin, sink);
        } else {
            nx::cvt::sink::adapter<uint8_t, std::vector<uint8_t>> sink(
                buffer[0]);
            nx::cvt::source::file f(input.c_str());
            nx::cvt::pipe(f, sink);
        }
    }

    std::vector<uint8_t>* src_v = &buffer[0];
    std::vector<uint8_t>* sink_v = &buffer[1];

    for (auto& filter_name : filters) {
        nx::cvt::source::bytes src { src_v->data(), src_v->size() };
        nx::cvt::sink::adapter<uint8_t, std::vector<uint8_t>> sink(*sink_v);
        if (filter_name == "hex_encode") {
            nx::cvt::pipe(src, nx::cvt::filter::hex_encode, sink);
        } else if (filter_name == "hex_decode") {
            nx::cvt::pipe(src, nx::cvt::filter::hex_decode, sink);
        } else if (filter_name == "zlib_compress") {
            nx::cvt::pipe(src, nx::cvt::filter::zlib_compress, sink);
        } else if (filter_name == "zlib_uncompress") {
            nx::cvt::pipe(src, nx::cvt::filter::zlib_uncompress, sink);
        } else if (filter_name == "") {
            continue;
        } else {
            NX_THROW("Unsupported codec " + filter_name);
        }
        nx::swap(src_v, sink_v);
        sink_v->clear();
    }

    {
        nx::cvt::source::bytes src { src_v->data(), src_v->size() };
        nx::cvt::pipe(src, nx::cvt::sink::cout);
    }

    return 0;
}