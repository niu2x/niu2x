cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(niu2x VERSION 0.1.0 LANGUAGES CXX C)

# use c++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# pic
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# hide all default
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# custom cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(find_dependencies)




add_library(niu2x "")
target_include_directories(niu2x 
	PUBLIC include ${CMAKE_BINARY_DIR}
	PRIVATE src
)

file(GLOB_RECURSE cpp_sources PATTERN "src/*.cpp")
target_sources(niu2x PRIVATE
	${cpp_sources}
)

file(GLOB_RECURSE c_source PATTERN "src/*.c")
target_sources(niu2x PRIVATE
	${c_source}
)

target_compile_definitions(niu2x PRIVATE niu2x_EXPORTS)

# include(dependent_prebuilt)

# dependent_prebuilt(
# 	TARGET niu2x 
# 	LIBS 
# 		zlib
# 		libuv
# 		openssl
# 		boost
# )

# list(APPEND runtime
# 	glfw
# 	GLEW::GLEW
#     OpenGL::OpenGL
# )

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
# 	list(APPEND runtime dl)
# 	list(APPEND runtime Threads::Threads)
# endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
# 	list(APPEND runtime wsock32 advapi32 iphlpapi psapi shell32 user32 userenv ws2_32)
# endif()


target_link_libraries(niu2x PUBLIC
	# ${runtime}
	ZLIB::ZLIB
	Boost::headers
	/usr/local/lib64/libcrypto.so
)

include(GenerateExportHeader)
generate_export_header(niu2x
	EXPORT_FILE_NAME niu2x/api.h
	EXPORT_MACRO_NAME API
	DEPRECATED_MACRO_NAME DEPRECATED_API
	INCLUDE_GUARD_NAME NIU2X_API_H
)


include(warning_as_error)
warning_as_error_enable(niu2x)

add_subdirectory(test)