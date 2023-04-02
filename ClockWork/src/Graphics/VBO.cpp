#pragma once
#include "VBO.h"

namespace CW
{
	VBO::VBO(const std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &Id);
		glBindBuffer(GL_ARRAY_BUFFER, Id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) , vertices.data(), GL_STATIC_DRAW);
	}

	VBO::VBO(const std::vector<glm::mat4>& matrices)
	{
		glGenBuffers(1, &Id);
		glBindBuffer(GL_ARRAY_BUFFER, Id);
		glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
	}

	void VBO::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, Id);
	}

	void VBO::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	VBO::~VBO()
	{
		glDeleteBuffers(1, &Id);
	}

}
