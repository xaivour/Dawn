cmake_minimum_required(VERSION 3.8)
project(externals)

add_subdirectory(spdlog EXCLUDE_FROM_ALL)

add_library(ext-spdlog INTERFACE)
target_link_libraries(ext-spdlog
    INTERFACE
        spdlog::spdlog
)

add_library(externals::spdlog ALIAS ext-spdlog)

find_package(OpenGL REQUIRED)

add_library(glad STATIC glad/src/glad.c)

target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/glad/include
)

target_link_libraries(glad
    PUBLIC
        ${OPENGL_LIBRARIES}
)

add_library(externals::glad ALIAS glad)

add_library(stb STATIC stb_image/stb_image.cpp stb_image/stb_image.h)

target_include_directories(stb
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/stb_image
)

add_library(externals::stb ALIAS stb)


