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

		VBO(const std::vector<Vertex>& vertices);
		VBO(const std::vector<glm::mat4>& matrices);
		~VBO();

		void Bind();
		void Unbind();

		GLuint Id;
	};
}