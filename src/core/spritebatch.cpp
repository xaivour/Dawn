#include "spritebatch.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Dawn
{
	SpriteBatch::SpriteBatch()
	{

	}

	SpriteBatch::~SpriteBatch()
	{

	}

	void SpriteBatch::registerBatch(GLuint texture, GLuint vertexArray, GLuint vertexBuffer, GLuint shaderProgram)
	{ 
		for(auto& b : m_batches)
		{
			if(b.texture == texture)
			return;
		}

		Batch batch;
		batch.texture = texture;
		batch.vertexArray = vertexArray;
		batch.vertexBuffer = vertexBuffer;
		batch.shaderProgram = shaderProgram;

		m_batches.emplace_back(batch);
	}

	void SpriteBatch::begin()
	{
		m_modelViewProj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	}

	void SpriteBatch::add(GLuint texture, const glm::vec2& pos, const glm::vec2& size)
	{
		static GLuint vertexArray;
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		static GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		static float verticies[] =
		{
           0.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 1.0f, 1.0f, 1.0f,

           1.0f, 1.0f, 1.0f, 1.0f,
           1.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		static const GLchar* vertexShaderSource = {
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
    
    static GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    static const GLchar* fragmentShaderSource = {
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
    
    static GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    static GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glUseProgram(shaderProgram);

    static glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    model = glm::scale(model, glm::vec3(size, 0.0f));

    m_modelViewProj = m_modelViewProj * model;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(m_modelViewProj));

    registerBatch(texture, vertexArray, vertexBuffer, shaderProgram);
	}

	void SpriteBatch::end()
	{
		glBindVertexArray(m_batches[0].vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}