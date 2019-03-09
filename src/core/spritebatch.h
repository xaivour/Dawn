#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Dawn
{
	struct Batch
	{
		GLuint texture;
		GLuint vertexArray;
		GLuint vertexBuffer;
		GLuint shaderProgram;
	};

	class SpriteBatch
	{
		std::vector<Batch> m_batches{};

		glm::mat4 m_modelViewProj{1.0f};

		void registerBatch(GLuint texture, GLuint vertexArray, GLuint vertexBuffer, GLuint shaderProgram);
	public:
		SpriteBatch();
		~SpriteBatch();

		void begin();
		void add(GLuint texture, const glm::vec2& pos, const glm::vec2& size);
		void end();
	};
}