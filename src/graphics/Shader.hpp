#ifndef SHADER_HPP
#define SHADER_HPP

/// @brief A shader program which is used by our OpenGL context to render.
/// @brief Is a wrapper for the GLSL shader program code. 
class Shader
{
public:
	Shader();
	~Shader();

	/// @brief Bind the shader to the OpenGL context.
	void bind();

	/// @brief Unbind the shader from the OpenGL context.
	void unbind();

	/// @brief Get the ID of the shader program, assuming it has been activated/initialized.
	/// @return 
	GLuint getShaderProgramId() const;

	/// @brief Set a uniform with the given name and integer value
	/// @param uniformBindingName 
	/// @param value 
	void updateIntUniform(const std::string& uniformBindingName, GLint value);

	/// @brief Set a uniform with the given name and 4D matrix value
	/// @param uniformBindingName 
	/// @param value 
	void updateMat4Uniform(const std::string& uniformBindingName, const glm::mat4& value);
private:
	std::string loadShader(const std::string& fname);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	GLuint m_shaderProgramId = 0; //Defaults to "no shader"
};

#endif
