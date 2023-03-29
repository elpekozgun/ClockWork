#pragma once

#include "glad/glad.h"
#include "Core/Core.h"

namespace CW
{
	class CW_API VBO
	{
	public:

		VBO(GLfloat* vertices, GLsizeiptr size);
		~VBO();

		void Bind();
		void Unbind();

		GLuint Id;
	};
}