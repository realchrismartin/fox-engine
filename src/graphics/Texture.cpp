#include "src/graphics/Texture.hpp"

#include "extern/stb_image.h"

Texture::Texture(const std::string& filePath) 
{
	// Create a texture object
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// Set texture parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//Ensure dat bad boi gets flipped vertically
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;

	unsigned char* imageData = stbi_load(filePath.data(), &width, &height, &nrChannels, 0);

	// Load texture data into the bound texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// Unbind the texture once it is loaded. We rebind it just before drawing using it.
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(imageData);
}

Texture::~Texture()
{
	if (m_textureID > 0)
	{
		glDeleteTextures(1, &m_textureID);
	}
}

void Texture::bind()
{
	if (m_textureID <= 0)
	{
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_textureID);

}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

