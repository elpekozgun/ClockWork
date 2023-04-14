#include "ShaderFactory.h"


namespace CW
{
	GLuint ShaderFactory::CreateShaderSource(const char* path, ShaderType type)
	{
		GLuint id = glCreateShader((int)type);
		auto source = Utilities::ReadFile(path);
		const char* shaderSource = source.c_str();

		glShaderSource(id, 1, &shaderSource, 0);
		glCompileShader(id);
		CheckShaderCompileError(id);

		return id;
	}

	Shader ShaderFactory::CreateShader(const std::string& name, std::vector<GLuint> shaderIds)
	{
		GLuint id = glCreateProgram();

		for (auto& shaderId : shaderIds)
		{
			glAttachShader(id, shaderId);
		}

		glLinkProgram(id);
		CheckLinkingError(id);

		for (auto& shaderId : shaderIds)
		{
			glDeleteShader(shaderId);
		}

		return Shader(name, id);
	}

	ComputeShader ShaderFactory::CreateComputeShader(const std::string& name, const char* path)
	{
		GLuint shaderId = CreateShaderSource(path, CW::ShaderType::Compute);

		GLuint id = glCreateProgram();

		glAttachShader(id, shaderId);
		glLinkProgram(id);
		CheckLinkingError(id);

		glDeleteShader(shaderId);
		return ComputeShader(name, id);
	}

	void ShaderFactory::CheckShaderCompileError(GLuint shader)
	{
		int success;
		char infolog[1024];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, sizeof(infolog), NULL, infolog);
			std::cout << "Compilation Error: " << infolog << "\n";
		}
	}

	void ShaderFactory::CheckLinkingError(GLuint program)
	{
		int success;
		char infolog[1024];

		glGetShaderiv(program, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, sizeof(infolog), NULL, infolog);
			std::cout << "Program Linking Error: " << infolog << "\n";
		}
	}
}

