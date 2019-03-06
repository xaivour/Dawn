#pragma once

#include <string>
#include <SDL.h>

namespace Dawn
{
    using uint32 = uint32_t;
    
    class AppState
    {
        SDL_Window* m_sdlWindow{};
        SDL_GLContext m_windowContext{};
    public:
        static AppState* create();
        
        void init(const std::string& title, uint32 width, uint32 height);
        
        void execute();
        
        bool isOpen() const;
    };
};
