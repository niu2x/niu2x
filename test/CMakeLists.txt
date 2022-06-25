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

include(GoogleTest)
gtest_add_tests(TARGET ${TEST})