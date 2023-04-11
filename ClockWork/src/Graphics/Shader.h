#pragma once 
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <string>
#include "Utility/Utilities.h"
#include <vector>
#include "Core/Core.h"
#include <unordered_map>

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
		Shader() {};

		void Delete();

		static GLuint CreateShaderSource(const char* path, ShaderType type);
		static Shader CreateShader(const std::string& name, std::vector<GLuint> shaderIds);

		void Use() const;
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetTexture(const std::string& name, GLenum textureType);
		void SetFloat(const std::string& name, float value);
		void setVec2(const std::string& name, const glm::vec2& value);
		void setVec2(const std::string& name, float x, float y);
		void setVec3(const std::string& name, const glm::vec3& value);
		void setVec3(const std::string& name, float x, float y, float z);
		void setVec4(const std::string& name, const glm::vec4& value);
		void setVec4(const std::string& name, float x, float y, float z, float w);
		void setMat2(const std::string& name, const glm::mat2& mat);
		void setMat3(const std::string& name, const glm::mat3& mat);
		void setMat4(const std::string& name, const glm::mat4& mat);
		
		std::string Name;
		GLuint Id;

	private:
		std::unordered_map<std::string, GLint> _uniformLocations;
		GLint& GetUniformLocation(const std::string& uniform);

		static void CheckShaderCompileError(GLuint shader);
		static void CheckLinkingError(GLuint program);
	};
}