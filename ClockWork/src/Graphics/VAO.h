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
		~VAO();

		void Unbind();
		void Bind();

		// stride and offset are element counts, not in bytes.
		template<typename T>
		void LinkAttribArray(VBO& vbo, GLuint layout, GLuint count, GLenum type, GLuint stride, GLuint offset)
		{
			vbo.Bind();
			glVertexAttribPointer(layout, count, type, GL_FALSE, sizeof(T) * stride, (void*) (sizeof(T) *offset));
			glEnableVertexAttribArray(layout);
			vbo.Unbind();
		}

		GLuint Id;
	};
}