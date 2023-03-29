#pragma once 
#include "glad/glad.h"
#include <string>
#include "Utility/Utilities.h"
#include <vector>
#include "Core/Core.h"

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

	class CW_API Shader
	{
	public:
		Shader(std::string name, GLuint id);
		~Shader();

		void Use() const;
		static GLuint CreateShaderSource(const char* path, ShaderType type);
		
		static std::unique_ptr<Shader> CreateShader(const std::string& name, std::vector<GLuint> shaderIds)
		{
			int id = glCreateProgram();

			for (auto& shaderId : shaderIds)
			{
				glAttachShader(id, shaderId);
			}

			glLinkProgram(id);

			for (auto& shaderId : shaderIds)
			{
				glDeleteShader(shaderId);
			}

			return std::make_unique<Shader>(name, id);
		}

		std::string Name;

	private:
		GLuint _id;
	};
}