#pragma once 

#include "glad/glad.h"
#include "Core/Core.h"

#include <vector>

namespace CW
{
	class CW_API EBO
	{
	public: 
		EBO(GLuint* indices, GLsizeiptr size);
		EBO(std::vector<GLuint> indices);
		~EBO();

		void Bind();
		void Unbind();

		GLuint Id;
	};
}