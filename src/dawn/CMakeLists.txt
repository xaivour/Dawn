# Should eventually be build as an library

add_executable(dawn
    main.cpp
)

target_link_libraries(dawn
    PRIVATE
        core
)
