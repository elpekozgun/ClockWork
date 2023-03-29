#include "Shader.h"

namespace CW 
{
	Shader::Shader(std::string name, GLuint id) : _id(id), Name(name)
	{

	}

	GLuint Shader::CreateShaderSource(const char* path, ShaderType type)
	{
		GLuint id = glCreateShader((int)type);
		auto source = Utilities::ReadFile(path);
		const char* shaderSource = source.c_str();

		glShaderSource(id, 1, &shaderSource, 0);
		glCompileShader(id);

		return id;
	}

	void Shader::Use() const
	{
		glUseProgram(_id);
	}

	Shader::~Shader()
	{
		glDeleteProgram(_id);
	}

}
