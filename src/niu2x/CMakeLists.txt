add_library(niu2x "")

target_include_directories(niu2x PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}/src
)

file(GLOB_RECURSE cpp_sources PATTERN "*.cpp")
target_sources(niu2x PRIVATE
	${cpp_sources}
)

file(GLOB_RECURSE c_source PATTERN "*.c")
target_sources(niu2x PRIVATE
	${c_source}
)

include(dependent_prebuilt)

dependent_prebuilt(
	TARGET niu2x 
	LIBS 
		luajit 
		zlib
		libuv
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	list(APPEND runtime dl)
	list(APPEND runtime Threads::Threads)
endif()

target_link_libraries(niu2x PUBLIC
	${runtime}
)

target_link_libraries(niu2x PUBLIC
	Boost::headers
)

include(GenerateExportHeader)
generate_export_header(niu2x
	EXPORT_FILE_NAME api.h
	EXPORT_MACRO_NAME API
	DEPRECATED_MACRO_NAME DEPRECATED_API
	INCLUDE_GUARD_NAME NIU2X_API_H
)


include(warning_as_error)
warning_as_error_enable(niu2x)