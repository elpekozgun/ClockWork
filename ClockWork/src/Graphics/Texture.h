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
		Texture(const char* path, const std::string& textureType, GLuint slot);
		Texture(const void* data, int width, int height, int channels, const std::string& textureType, GLuint slot);
		
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