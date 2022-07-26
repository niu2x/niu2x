add_executable(nx "")

file(GLOB_RECURSE cpp_source "src/*.cpp")
target_sources(nx PRIVATE ${cpp_source})

file(GLOB_RECURSE c_source "src/*.c")
target_sources(nx PRIVATE ${c_source})

target_link_libraries(nx PRIVATE niu2x::niu2x)