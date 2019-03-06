
add_library(core STATIC
    log.cpp
    log.h
    app_state.cpp
    app_state.h
)

target_link_libraries(core
    PUBLIC
        externals::glad
        externals::spdlog
        SDL2::SDL2main
        SDL2::SDL2
)
