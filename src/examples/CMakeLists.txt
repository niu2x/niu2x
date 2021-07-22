function(build_application name)
	add_executable(${name} "")
	file(GLOB_RECURSE sources PATTERN "${name}/*.cpp")
	target_sources(${name} PRIVATE ${sources})
	target_link_libraries(${name} PRIVATE niu2x)
endfunction()

build_application(demo)
build_application(zlib)
