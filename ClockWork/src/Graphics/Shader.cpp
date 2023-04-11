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

	Shader Shader::CreateShader(const std::string& name, std::vector<GLuint> shaderIds)
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

		return Shader(name, id);
	}

	void Shader::Use() const
	{
		glUseProgram(Id);
	}

	void Shader::SetBool(const std::string& name, bool value) 
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void Shader::SetInt(const std::string& name, int value) 
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetTexture(const std::string& name, GLenum slot) 
	{
		glUniform1i(GetUniformLocation(name), slot);
	}

	void Shader::SetFloat(const std::string& name, float value) 
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value) 
	{
		glUniform2fv(GetUniformLocation(name), 1, &value[0]);
	}

	void Shader::setVec2(const std::string& name, float x, float y) 
	{
		glUniform2f(GetUniformLocation(name), x, y);
	}
	
	void Shader::setVec3(const std::string& name, const glm::vec3& value) 
	{
		glUniform3fv(GetUniformLocation(name), 1, &value[0]);
	}

	void Shader::setVec3(const std::string& name, float x, float y, float z) 
	{
		glUniform3f(GetUniformLocation(name), x, y, z);
	}
	
	void Shader::setVec4(const std::string& name, const glm::vec4& value) 
	{
		glUniform4fv(GetUniformLocation(name), 1, &value[0]);
	}

	void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}
	
	void Shader::setMat2(const std::string& name, const glm::mat2& mat) 
	{
		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
	
	void Shader::setMat3(const std::string& name, const glm::mat3& mat) 
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
	
	void Shader::setMat4(const std::string& name, const glm::mat4& mat) 
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
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

	GLint& Shader::GetUniformLocation(const std::string& uniform)
	{
		if (_uniformLocations.find(uniform) == _uniformLocations.end())
		{
			_uniformLocations[uniform] = glGetUniformLocation(Id, uniform.c_str());
		}
		return _uniformLocations[uniform];
	}

	void Shader::Delete()
	{
		glDeleteProgram(Id);
	}
}
