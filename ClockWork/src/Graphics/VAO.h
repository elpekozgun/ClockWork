#pragma once 
#include "glad/glad.h"
#include "Core/Core.h"
#include "VBO.h"

namespace CW
{
	class CW_API VAO
	{
	public:
		VAO();

		void Unbind();
		void Bind();
		void Delete();

		// stride and offset are element counts, not in bytes.
		template<typename T>
		void LinkAttribArray(VBO& vbo, GLuint layout, GLuint count, GLenum type, GLuint stride, GLuint offset)
		{
			vbo.Bind();
			glEnableVertexAttribArray(layout);
			glVertexAttribPointer(layout, count, type, GL_FALSE, sizeof(T) * stride, (void*) (sizeof(T) *offset));
			vbo.Unbind();
		}

		void AddInstanceBuffers(VBO& vbo)
		{
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
		}

		GLuint Id;
	};
}