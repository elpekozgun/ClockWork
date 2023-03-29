#pragma once
#include "VAO.h"

namespace CW
{
	VAO::VAO()
	{
		glGenVertexArrays(1, &Id);
	}

	void VAO::Bind()
	{
		glBindVertexArray(Id);
	}

	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &Id);
	}

}
