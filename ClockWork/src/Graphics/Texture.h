#pragma once
#include "Core/Core.h"
#include "stb_image.h"
#include "glad/glad.h"
#include <string>
#include <iostream>

namespace CW
{
	class CW_API Texture
	{
	public:
		Texture(const std::string& path, GLenum TextureType, GLuint slot, GLenum format, GLenum pixelType, GLint filterMode, GLint wrapMode);
		~Texture();
		

		void Bind();
		void Unbind();

		int Width = 0;
		int Height = 0;
		int ChannelCount = 0;
		int Slot = 0;
		GLenum TextureType;
		GLuint TextureId;
	};
}