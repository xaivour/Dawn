#include <glad/glad.h>
#include "app_state.h"
#include "log.h"

namespace Dawn
{
    
AppState * AppState::create()
{
    return new AppState();
}

void AppState::init(const std::string& title, Dawn::uint32 width, Dawn::uint32 height)
{
   bool success = SDL_Init(SDL_INIT_EVERYTHING);
   DAWN_INTERNAL_ASSERT(success == 0, "SDL not initialized");
    
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
   
   m_sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
   DAWN_INTERNAL_ASSERT(m_sdlWindow != nullptr, "Couldn't create SDL window");
   
   SDL_ShowWindow(m_sdlWindow);
   
   m_windowContext = SDL_GL_CreateContext(m_sdlWindow);
   DAWN_INTERNAL_ASSERT(m_windowContext != nullptr, "Couldn't create valid OpenGL context");
   
   success = gladLoadGLES2Loader(SDL_GL_GetProcAddress);
   DAWN_INTERNAL_ASSERT(success != 0, "Couldn't initialize glad");
   
   SDL_GL_MakeCurrent(m_sdlWindow, m_windowContext);
   SDL_GL_SwapWindow(m_sdlWindow);
}

void AppState::execute()
{
    while(isOpen())
    {
       glClearColor(1, 0.75, 0.25, 1);
       glClear(GL_COLOR_BUFFER_BIT);
       SDL_GL_SwapWindow(m_sdlWindow);
    }
}

bool AppState::isOpen() const
{
    static SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
            return false;
    }
    return true;
}

}
