#pragma once

#include "glad/glad.h"
#include "Core/Core.h"
#include "Vertex.h"

#include <vector>

namespace CW
{
	class CW_API VBO
	{
	public:

		VBO(GLfloat* vertices, GLsizeiptr size);
		VBO(const std::vector<Vertex>& vertices);
		~VBO();

		void Bind();
		void Unbind();

		GLuint Id;
	};
}