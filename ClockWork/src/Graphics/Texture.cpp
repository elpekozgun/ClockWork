#include "Texture.h"

namespace CW
{
	Texture::Texture(const char* path, const std::string& textureType, GLuint slot, GLenum format, GLenum pixelType, GLint filterMode, GLint wrapMode)
	{
		try
		{
			stbi_set_flip_vertically_on_load(true);
			unsigned char* bytes = stbi_load(path, &Width, &Height, &ChannelCount, 0);

			Slot = slot;
			TextureType = textureType;

			glGenTextures(1, &TextureId);
			glBindTexture(GL_TEXTURE_2D, TextureId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, pixelType, (void*)bytes);
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

