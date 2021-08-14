add_library(niu2x_lua STATIC "")

target_include_directories(niu2x_lua PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}/src
)

file(GLOB_RECURSE cpp_sources PATTERN "*.cpp")
target_sources(niu2x_lua PRIVATE
	${cpp_sources}
)

file(GLOB_RECURSE c_source PATTERN "*.c")
target_sources(niu2x_lua PRIVATE
	${c_source}
)

include(dependent_prebuilt)

dependent_prebuilt(
	TARGET niu2x_lua 
	LIBS 
		luajit
)

target_link_libraries(niu2x_lua PUBLIC
	niu2x
)


include(warning_as_error)
warning_as_error_enable(niu2x_lua)