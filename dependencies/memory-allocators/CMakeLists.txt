file(GLOB_RECURSE sources PATTERN "src/*.cpp")

add_library(memory-allocators STATIC
	${sources} 
)

target_include_directories(memory-allocators PUBLIC include)