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

# custom cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(find_dependencies)

# niu2x runtime library
add_subdirectory(src/niu2x)

add_subdirectory(src/examples)



