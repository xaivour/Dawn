
add_library(core STATIC
    log.cpp
    log.h
    app_state.h
    spritebatch.cpp
    spritebatch.h
    events/events.cpp 
    events/events.h
    events/event_handler.h 
    events/event_listener.h 
    events/key_events.h 
    events/mouse_events.h
)

target_compile_features(core 
    PUBLIC
        cxx_std_11
)

target_link_libraries(core
    PUBLIC
        externals::glad
        externals::stb
        externals::glm
        externals::spdlog
        SDL2::SDL2main
        SDL2::SDL2
)

target_compile_options(core
    PUBLIC 
        -no-pie
        -Wno-reorder
)

# platform specific sources
add_subdirectory(platform) 

target_link_libraries(core
    PRIVATE 
        platform
)
