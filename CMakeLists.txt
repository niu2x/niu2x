cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project(niu2x VERSION 0.1.0 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(utils)

# add library
set(LIB_NAME "niu2x")
add_library(${LIB_NAME} "")

file(GLOB_RECURSE cpp_source "src/*.cpp")
target_sources(${LIB_NAME} PRIVATE ${cpp_source})

file(GLOB_RECURSE c_source "src/*.c")
target_sources(${LIB_NAME} PRIVATE ${c_source})

target_include_directories(${LIB_NAME} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/niu2x/3rd>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_include_directories(${LIB_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
set_target_properties(niu2x PROPERTIES
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

include(GenerateExportHeader)
generate_export_header(niu2x
	EXPORT_FILE_NAME include/niu2x/api.h
	EXPORT_MACRO_NAME NXAPI
	DEPRECATED_MACRO_NAME DEPRECATED_API
	INCLUDE_GUARD_NAME NX_API_H
)

# find_package(ZLIB)
# find_package(libuv)
# find_package(CURL)

# find_package(OpenSSL 3)
# find_package(FFmpeg)

if(FFmpeg_FOUND)
	list(APPEND thirdparty_LIBS 
		FFmpeg::avformat
		FFmpeg::avcodec
		FFmpeg::avutil
		FFmpeg::swresample
	)
endif()

if(OpenSSL_FOUND)
	list(APPEND thirdparty_LIBS 
		OpenSSL::Crypto
		OpenSSL::SSL
	)

	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		list(APPEND thirdparty_LIBS dl)
	endif()
endif()

if(CURL_FOUND)
	list(APPEND thirdparty_LIBS 
		curl
	)
endif()

if(libuv_FOUND)
	list(APPEND thirdparty_LIBS 
		uv_a
	)
endif()


if(ZLIB_FOUND)
	list(APPEND thirdparty_LIBS 
		ZLIB::ZLIB
	)
endif()


message(STATUS "NX ZLIB_FOUND ${ZLIB_FOUND}")
message(STATUS "NX libuv_FOUND ${libuv_FOUND}")
message(STATUS "NX CURL_FOUND ${CURL_FOUND}")
message(STATUS "NX OpenSSL_FOUND ${OpenSSL_FOUND}")
message(STATUS "NX FFmpeg_FOUND ${FFmpeg_FOUND}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/build.h.in include/niu2x/build.h)

target_link_libraries(niu2x PUBLIC 
	${thirdparty_LIBS}
)


warning_as_error_enable(niu2x)

# library alias
add_library(${PROJECT_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})


# install & package 
include(GNUInstallDirs)

install(TARGETS ${LIB_NAME} 
	EXPORT niu2x_export 
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY 
	include/
	${CMAKE_CURRENT_BINARY_DIR}/include/
	TYPE INCLUDE
)

install(EXPORT niu2x_export 
	FILE niu2x-config.cmake 
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake 
	NAMESPACE niu2x::
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"niu2x-config-version.cmake"
	COMPATIBILITY SameMajorVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/niu2x-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake 
)


# cpack
set(CPACK_GENERATOR ZIP)
include(CPack)


# test
include(CTest)
if(BUILD_TESTING)
	set(INSTALL_GTEST OFF)
	add_subdirectory(lib/googletest)
	add_subdirectory(test)
endif()

