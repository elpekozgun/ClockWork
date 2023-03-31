#include "Texture.h"

namespace CW
{
	Texture::Texture(const std::string & path, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType, GLint filterMode, GLint wrapMode)
	{
		try
		{
			stbi_set_flip_vertically_on_load(true);
			unsigned char* bytes = stbi_load(path.c_str(), &Width, &Height, &ChannelCount, 0);

			Slot = slot;
			TextureType = textureType;

			glGenTextures(1, &TextureId);
			glBindTexture(textureType, TextureId);

			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filterMode);
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filterMode);
			glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapMode);

			glTexImage2D(textureType, 0, format, Width, Height, 0, format, pixelType, (void*)bytes);
			glGenerateMipmap(textureType);

			stbi_image_free(bytes);
			glBindTexture(textureType, 0);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << "\n";
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &TextureId);
	}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0 + Slot);
		glBindTexture(TextureType, TextureId);
	}

	void Texture::Unbind()
	{
		glBindTexture(TextureType, 0);
	}

}

