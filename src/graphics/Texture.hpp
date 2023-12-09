#ifndef TEXTURE_HPP
#define TEXTURE_HPP

/// @brief A wrapper for an image which is loaded into the GPU memory and sampled from to color fragments.
class Texture
{
public:
	Texture();
	~Texture();

	/// @brief Initialize the texture and tell the GPU about it. It will be unbound when done initializing.
	void activate();

	/// @brief Bind the loaded texture to the OpenGL context.
	void bind();

	/// @brief Unbind the loaded texture from the OpenGL context.
	void unbind();

private:
	GLuint m_textureID = 0;
};

#endif
