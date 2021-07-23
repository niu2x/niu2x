#include <niu2x/io.h>

#include <common/cxxopts.h>

int main(int argc, char* argv[])
{
    cxxopts::Options options("cvt", "niu2x's cvt tools");

    options.add_options()("o,output", "output", cxxopts::value<std::string>())(
        "i,input", "input", cxxopts::value<std::string>())(
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

    if (codec == "") {
        nx::io::pipe(source, sink);
    } else {
        auto filters = nx::string_utils::split(codec, '|');
    }

    return 0;
}