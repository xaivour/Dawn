# all platform hazard will endup being here
set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_library(platform INTERFACE)

target_sources(platform 
	INTERFACE
	    ${DIR}/sdl/sdl_application.cpp
		${DIR}/sdl/sdl_application.h
)

