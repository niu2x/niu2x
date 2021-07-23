#include <niu2x/io.h>

#include <common/cxxopts.h>

int main(int argc, char* argv[]) { 
	cxxopts::Options options("MyProgram", "One line description of MyProgram");

	options.add_options()
        ("o,output", "output", cxxopts::value<std::string>())
        ("i,input", "input", cxxopts::value<std::string>())
        ("h,help", "Print usage")
    ;

	auto result = options.parse(argc, argv);


	return 0; 
}