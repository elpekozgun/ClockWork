#pragma once

#include <string>

#include "Core/Core.h"
#include "glad/glad.h"
#include "Shader.h"
#include "ComputeShader.h"

namespace CW
{
	enum class CW_API ShaderType
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Compute = GL_COMPUTE_SHADER,
		TessEval = GL_TESS_EVALUATION_SHADER,
		TessControl = GL_TESS_CONTROL_SHADER
	};

	class CW_API ShaderFactory
	{
	public:
		static GLuint CreateShaderSource(const char* path, ShaderType type);
		static Shader CreateShader(const std::string& name, std::vector<GLuint> shaderIds);
		static ComputeShader CreateComputeShader(const std::string& name, const char* path);
		static void CheckShaderCompileError(GLuint shader);
		static void CheckLinkingError(GLuint program);

	};
}
