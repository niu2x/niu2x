add_library(niu2x SHARED "")

target_include_directories(niu2x PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}/src
)

file(GLOB_RECURSE sources PATTERN "*.cpp")

target_sources(niu2x PRIVATE
	${sources}
)

include(dependent_prebuilt)

dependent_prebuilt(
	TARGET niu2x 
	LIBS 
		luajit 
)

target_link_libraries(niu2x PRIVATE
	memory-allocators
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