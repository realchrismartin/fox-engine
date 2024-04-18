#ifndef TEXTURE_HPP
#define TEXTURE_HPP

/// @brief A wrapper for an image which is loaded into the GPU memory and sampled from to color fragments.

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	/// @brief Bind the loaded texture to the OpenGL context.
	void bind();

	/// @brief Unbind the loaded texture from the OpenGL context.
	void unbind();

private:
	GLuint m_textureID = 0;
};

#endif
