#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "app_state.h"
#include "spritebatch.h"
#include "log.h"

namespace Dawn
{

const stbi_uc* pixels{};

SpriteBatch g_spriteBatch{};
uint32 g_texture{};

void glInit()
{
    stbi_set_flip_vertically_on_load(false);
    
    int x, y, n;
    pixels = stbi_load("hello.png", &x, &y, &n, 0);
    DAWN_ASSERT(pixels != nullptr, "ERROR loading texture");
    
    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    g_spriteBatch.begin();
    g_spriteBatch.add(g_texture, glm::vec2(20.0f, 20.0f), glm::vec2(32.0f, 32.0f));
};
    
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
   
   glInit();
}

void AppState::execute()
{
    while(isOpen())
    {
       glClearColor(0, 0.75, 0.25, 1);
       glClear(GL_COLOR_BUFFER_BIT);
       g_spriteBatch.end();
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
