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

		GLuint Id;
	};
}