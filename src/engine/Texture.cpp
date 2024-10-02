#include "engine/Texture.hpp"

Texture::Texture(std::string textureFile, int textureWrapX, int textureWrapY, int textureMinFilter, int textureMaxFilter, bool isGeneratingMipmaps)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMaxFilter);

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* pixelData = stbi_load(textureFile.c_str(), &width, &height, &channels, 0);
	NON_CRITICAL_ASSERT(pixelData, "Image didn't load");

	int internalFormat = (channels == 4) ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, pixelData);

	if (isGeneratingMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(pixelData);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}


void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::activate(unsigned int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, m_id);
}


unsigned int Texture::getId()
{
	return m_id;
}