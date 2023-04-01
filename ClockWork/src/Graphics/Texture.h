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
		Texture(const char* path, const std::string& TextureType, GLuint slot, GLenum format, GLenum pixelType, GLint filterMode, GLint wrapMode);
		
		void Bind();
		void Unbind();
		void Delete();

		int Width = 0;
		int Height = 0;
		int ChannelCount = 0;
		int Slot = 0;
		std::string TextureType;
		GLuint TextureId;
	};
}