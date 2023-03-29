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

		template<typename T>
		void LinkVBO(VBO& vbo, GLuint layout, GLuint size)
		{
			vbo.Bind();
			glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, size * sizeof(T), (void*)0);
			glEnableVertexAttribArray(layout);
			vbo.Unbind();
		}

		GLuint Id;
	};
}