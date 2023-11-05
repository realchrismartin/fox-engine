#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "src/extern/stb_image.h"

class Texture
{
public:
	Texture()
	{
	}

	~Texture()
	{
		if (m_textureID > 0)
		{
			glDeleteTextures(1, &m_textureID);
		}
	}

	void activate()
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
		unsigned char* imageData = stbi_load("../../img/sprite_sheet.png", &width, &height, &nrChannels, 0);

		// Load texture data into the bound texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		// Unbind the texture once it is loaded. We rebind it just before drawing using it.
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(imageData);
	}

	void bind()
	{
		if (m_textureID <= 0)
		{
			return;
		}

		glBindTexture(GL_TEXTURE_2D, m_textureID);

	}

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
private:
	GLuint m_textureID = 0;
};

#endif
