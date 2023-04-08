#include "Texture.h"

namespace CW
{
	Texture::Texture(const char* path, const std::string& textureType, GLuint slot)
	{
		try
		{
			//stbi_set_flip_vertically_on_load(true);
			unsigned char* bytes = stbi_load(path, &Width, &Height, &ChannelCount, 0);

			GLenum format = GL_RGBA;
			if (ChannelCount == 1) 
			{
				format = GL_RED;
			}
			else if (ChannelCount == 3)
			{
				format = GL_RGB;
			}
			else if (ChannelCount == 4)
			{
				format = GL_RGBA;
			}


			Slot = slot;
			TextureType = textureType;

			glGenTextures(1, &TextureId);

			glActiveTexture(GL_TEXTURE0 + Slot);
			glBindTexture(GL_TEXTURE_2D, TextureId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, (void*)bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(bytes);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(0);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << "\n";
		}
	}

	Texture::Texture(const void* bytes, int width, int height, int channels, const std::string& textureType, GLuint slot)
	{
		try
		{
			//stbi_set_flip_vertically_on_load(true);
			//unsigned char* bytes = stbi_load(path, &Width, &Height, &ChannelCount, 0);

			GLenum format = GL_RGBA;
			if (ChannelCount == 1)
			{
				format = GL_RED;
			}
			else if (ChannelCount == 3)
			{
				format = GL_RGB;
			}
			else if (ChannelCount == 4)
			{
				format = GL_RGBA;
			}


			Slot = slot;
			TextureType = textureType;

			glGenTextures(1, &TextureId);

			glActiveTexture(GL_TEXTURE0 + Slot);
			glBindTexture(GL_TEXTURE_2D, TextureId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, (void*)bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(0);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << "\n";
		}
	}


	//Texture::~Texture()
	//{
	//	glDeleteTextures(1, &TextureId);
	//}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0 + Slot);
		glBindTexture(GL_TEXTURE_2D, TextureId);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Delete()
	{
		glDeleteTextures(1, &TextureId);
	}
}

