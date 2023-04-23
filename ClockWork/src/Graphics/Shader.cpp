#include "Shader.h"

namespace CW 
{
	Shader::Shader(std::string name, GLuint id) : Id(id), Name(name)
	{

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

	void Shader::SetUInt(const std::string& name, unsigned int value)
	{
		glUniform1ui(GetUniformLocation(name), value);
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
