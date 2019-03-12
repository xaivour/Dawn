#include "sdl_application.h"
#include <glad/glad.h>
#include "core/app_state.h"
#include "core/common.h"
 
namespace Dawn
{
	void SdlApplication::sdlInit()
	{
		uint32 initFlags = SDL_INIT_EVERYTHING;
		bool initialized = SDL_WasInit(initFlags);

		if(initialized != initFlags && SDL_Init(initFlags) != 0) {
		    DAWN_INTERNAL_ERROR("SDL Initialization failed: %s", SDL_GetError());
		}
	}

	void SdlApplication::initWindow(const std::string& title, uint32 width, uint32 height)
	{
		bool success = 0;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
   
        sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
        DAWN_INTERNAL_ASSERT(sdlWindow != nullptr, "Couldn't create SDL window");
   
        SDL_ShowWindow(sdlWindow);
   
        windowContext = SDL_GL_CreateContext(sdlWindow);
        DAWN_INTERNAL_ASSERT(windowContext != nullptr, "Couldn't create valid OpenGL context");
   
        success = gladLoadGLES2Loader(SDL_GL_GetProcAddress);
        DAWN_INTERNAL_ASSERT(success != 0, "Couldn't initialize glad");
   
        SDL_GL_MakeCurrent(sdlWindow, windowContext);
        SDL_GL_SwapWindow(sdlWindow);
	}

	AppState * AppState::create()
    {
       return new SdlApplication();
    }

	SdlApplication* SdlApplication::create()
	{
		return new SdlApplication;
	}

	void SdlApplication::execute()
	{
		isAppRunning = true;
		EventHandler tempEventHandler;
		while(isAppRunning) {
			processEvents(tempEventHandler);
		}
	}

	inline bool SdlApplication::isRunning() const
	{
		return isAppRunning;
	}

	void SdlApplication::processEvents(EventHandler& eventHandler)
	{
		static SDL_Event e;
		while(SDL_PollEvent(&e)) {
		   switch(e.type){
		      case SDL_KEYDOWN:
			    eventHandler.onKeyDown(e.key.keysym.scancode, e.key.repeat != 0);
			    break;
		      case SDL_KEYUP:
			    eventHandler.onKeyUp(e.key.keysym.scancode, e.key.repeat != 0);
			    break;
		      case SDL_MOUSEBUTTONDOWN:
			    eventHandler.onMouseDown(e.button.button, e.button.clicks);
			    break;
		      case SDL_MOUSEBUTTONUP:
			    eventHandler.onMouseUp(e.button.button, e.button.clicks);
			    break;
		      case SDL_MOUSEMOTION:
			    eventHandler.onMouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
			    break;
		      case SDL_QUIT:
			    isAppRunning = false;
			    break;
		      default:
			    break;
		};
	    }
	}

}

