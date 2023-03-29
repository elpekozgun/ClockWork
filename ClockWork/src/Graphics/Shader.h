#pragma once 
#include "glad/glad.h"
#include "glm.hpp"
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

		static GLuint CreateShaderSource(const char* path, ShaderType type);
		static std::unique_ptr<Shader> CreateShader(const std::string& name, std::vector<GLuint> shaderIds);

		void Use() const;
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w);
		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		
		std::string Name;
		GLuint Id;

	private:
		static void CheckShaderCompileError(GLuint shader);
		static void CheckLinkingError(GLuint program);
	};
}