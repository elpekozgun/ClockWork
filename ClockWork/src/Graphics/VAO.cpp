#pragma once
#include "VAO.h"
#include <iostream>

namespace CW
{
	VAO::VAO()
	{
		std::cout << "created vao\n";
		glGenVertexArrays(1, &Id);
	}

	//void VAO::LinkAttribArray(VBO& vbo, GLuint layout, GLuint count, GLenum type, GLsizeiptr stride, GLuint offset)
	//{
	//	vbo.Bind();
	//	glVertexAttribPointer(layout, count, type, GL_FALSE, stride, (void*)offset);
	//	glEnableVertexAttribArray(layout);
	//	vbo.Unbind();
	//}


	void VAO::Bind()
	{
		glBindVertexArray(Id);
	}

	void VAO::Delete()
	{
		glDeleteVertexArrays(1, &Id);
	}

	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}

	VAO::~VAO()
	{
	}

}
