#pragma once

#include "Core/core.h"
#include <string>
#include "glad/glad.h"
#include "Shader.h"

namespace CW
{
	class ComputeShader : public Shader
	{
	public:
		using Shader::Shader;

		inline void Dispatch(GLuint x, GLuint y, GLuint z, GLbitfield barrierBit)
		{
			glDispatchCompute(x,y,z);
			glMemoryBarrier(barrierBit);
		}
	};
}