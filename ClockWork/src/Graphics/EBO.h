#pragma once 

#include "glad/glad.h"
#include "Core/Core.h"

namespace CW
{
	class CW_API EBO
	{
	public: 
		EBO(GLuint* vertices, GLsizeiptr size);
		~EBO();

		void Bind();
		void Unbind();

		GLuint Id;
	};
}