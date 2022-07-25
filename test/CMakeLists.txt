set(TEST ${PROJECT_NAME}_test)

add_executable(${TEST} "")
target_link_libraries(${TEST} PRIVATE 
	gtest
	${LIB_NAME}
)

file(GLOB_RECURSE cpp_source "*.cpp")
target_sources(${TEST} PRIVATE
	${cpp_source}
)

target_compile_definitions(${TEST} PRIVATE -DNXDIR="${CMAKE_CURRENT_SOURCE_DIR}/")

include(GoogleTest)
gtest_add_tests(TARGET ${TEST})