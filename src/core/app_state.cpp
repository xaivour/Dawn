#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "app_state.h"
#include "log.h"

namespace Dawn
{

const stbi_uc* pixels{};

uint32 g_vertexArrayObject{};    
uint32 g_vertexBuffer{};
uint32 g_indexBuffer{};
uint32 g_vertexShader{};
uint32 g_fragmentShader{};
uint32 g_shaderProgram{};
uint32 g_texture{};

glm::mat4 g_model{glm::mat4(1.0f)};
glm::mat4 g_view{glm::mat4(1.0f)};
glm::mat4 g_projection{glm::mat4(1.0f)};

void glInit()
{
    stbi_set_flip_vertically_on_load(false);
    
    int x, y, n;
    pixels = stbi_load("hello.png", &x, &y, &n, 0);
    DAWN_ASSERT(pixels != nullptr, "ERROR loading texture");

    //g_model = glm::translate(g_model, glm::vec3(20.0f, 20.0f, 0.0f));  

    g_model = glm::scale(g_model, glm::vec3(600.0f, 400.0f, 1.0f));
    
    g_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    
    g_view = g_projection * g_model;
    
    glGenVertexArrays(1, &g_vertexArrayObject);
    glBindVertexArray(g_vertexArrayObject);
    
    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    float vertices[] = 
    {
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 
        1.0f, 1.0f, 1.0f, 1.0f
    };
    
    glGenBuffers(1, &g_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    uint32 indicies[] = 
    {
        0, 1, 2,
        2, 3, 0
    };
    
    glGenBuffers(1, &g_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
    
    const GLchar* vertexShaderSource = {
        R"(
            #version 300 es
            
            layout (location = 0) in vec2 pos;
            layout (location = 1) in vec2 uvIn;
            
            uniform mat4 mvp;
            
            out vec2 uv;
            
            void main()
            {
                gl_Position = mvp * vec4(pos, 0, 1.0f);
                uv = uvIn;
            }
        )"
    };
    
    g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(g_vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(g_vertexShader);
    
     const GLchar* fragmentShaderSource = {
        R"(
            #version 300 es
            
            precision mediump float;
            
            out vec4 color;
            in vec2 uv;
            
            uniform sampler2D tex;
            
            void main()
            {
                color = texture(tex, uv);
            }
        )"
    };
    
    g_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(g_fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(g_fragmentShader);
    
    g_shaderProgram = glCreateProgram();
    glAttachShader(g_shaderProgram, g_vertexShader);
    glAttachShader(g_shaderProgram, g_fragmentShader);
    glLinkProgram(g_shaderProgram);
    
    glUseProgram(g_shaderProgram);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    
    glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(g_view));
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
       glBindTexture(GL_TEXTURE_2D, g_texture);
       glBindVertexArray(g_vertexArrayObject);
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
