#pragma once 

#include "EBO.h"

namespace CW
{

	EBO::EBO(GLuint* indices, GLsizeiptr size)
	{
		glGenBuffers(1, &Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

	}
	EBO::EBO(std::vector<GLuint> indices)
	{
		glGenBuffers(1, &Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}


	void EBO::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
	}

	void EBO::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}


	EBO::~EBO()
	{
		glDeleteBuffers(1, &Id);
	}

}
