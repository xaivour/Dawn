cmake_minimum_required(VERSION 3.8)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(dawn VERSION 0.1.0)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)

find_package(SDL2 CONFIG REQUIRED)

set(DAWN_DEBUG_MODE ON CACHE BOOL "Enable Debug mode")

if(DAWN_DEBUG_MODE)
set(DAWN_DEBUG 1)
else()
set(DAWN_DEBUG 0)
endif()

configure_file(
	${CMAKE_SOURCE_DIR}/src/config.h.in
	${CMAKE_BINARY_DIR}/src/config.h
)

include_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(externals EXCLUDE_FROM_ALL)
add_subdirectory(src)
