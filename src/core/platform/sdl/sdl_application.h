#pragma once 

#include <SDL.h>
#include "core/app_state.h"

namespace Dawn
{
	class SdlApplication : public AppState
	{
		bool isAppRunning{};
		SDL_Window* sdlWindow{};
		SDL_GLContext windowContext{};
	public:
		static SdlApplication* create();

		void sdlInit();

        void initWindow(const std::string& title, uint32 width, uint32 height) override; 
        inline bool isRunning() const override;
        uint32 getFps() const override { /* TODO: Implementation */ };
        void execute() override;
        
        void processEvents(EventHandler& eventHandler) override;
	};
}

