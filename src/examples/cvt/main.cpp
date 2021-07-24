#include <niu2x/io.h>

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
    auto& source = nx::io::source::cin;
    auto& sink = nx::io::sink::cout;

    auto codec = result["codec"].as<std::string>();

    auto filters = nx::string_utils::split(codec, '|');
    std::vector<uint8_t> buffer[2];

    {
        std::string input = result["input"].as<std::string>();
        if (input == "") {
            nx::io::sink::adapter<uint8_t, std::vector<uint8_t>> sink(
                buffer[0]);
            nx::io::pipe(nx::io::source::cin, sink);
        } else {
            nx::io::sink::adapter<uint8_t, std::vector<uint8_t>> sink(
                buffer[0]);
            nx::io::source::file f(input.c_str());
            nx::io::pipe(f, sink);
        }
    }

    std::vector<uint8_t>* src_v = &buffer[0];
    std::vector<uint8_t>* sink_v = &buffer[1];

    for (auto& filter_name : filters) {
        nx::io::source::bytes src { src_v->data(), src_v->size() };
        nx::io::sink::adapter<uint8_t, std::vector<uint8_t>> sink(*sink_v);
        if (filter_name == "hex_encode") {
            nx::io::pipe(src, nx::io::filter::hex_encode, sink);
        } else if (filter_name == "hex_decode") {
            nx::io::pipe(src, nx::io::filter::hex_decode, sink);
        } else if (filter_name == "zlib_compress") {
            nx::io::pipe(src, nx::io::filter::zlib_compress, sink);
        } else if (filter_name == "zlib_uncompress") {
            nx::io::pipe(src, nx::io::filter::zlib_uncompress, sink);
        } else if (filter_name == "") {
            continue;
        } else {
            NX_THROW("Unsupported codec " + filter_name);
        }
        nx::swap(src_v, sink_v);
        sink_v->clear();
    }

    {
        nx::io::source::bytes src { src_v->data(), src_v->size() };
        nx::io::pipe(src, nx::io::sink::cout);
    }

    return 0;
}