#include "Shader.h"

namespace CW 
{
	Shader::Shader(std::string name, GLuint id) : Id(id), Name(name)
	{

	}

	GLuint Shader::CreateShaderSource(const char* path, ShaderType type)
	{
		GLuint id = glCreateShader((int)type);
		auto source = Utilities::ReadFile(path);
		const char* shaderSource = source.c_str();

		glShaderSource(id, 1, &shaderSource, 0);
		glCompileShader(id);
		CheckShaderCompileError(id);

		return id;
	}

	std::unique_ptr<Shader> Shader::CreateShader(const std::string& name, std::vector<GLuint> shaderIds)
	{
		int id = glCreateProgram();

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

		return std::make_unique<Shader>(name, id);
	}

	void Shader::Use() const
	{
		glUseProgram(Id);
	}

	void Shader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y);
	}
	
	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		unsigned int location = glGetUniformLocation(Id, name.c_str());
		glUniform3fv(location, 1, &value[0]);
	}

	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
	}
	
	void Shader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w);
	}
	
	void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	
	void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	
	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


	void Shader::CheckShaderCompileError(GLuint shader)
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

	void Shader::CheckLinkingError(GLuint program)
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


	Shader::~Shader()
	{
		glDeleteProgram(Id);
	}

}
