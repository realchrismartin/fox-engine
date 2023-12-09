#ifndef SHADER_HPP
#define SHADER_HPP

<<<<<<< HEAD
=======
/// @brief A shader program which is used by our OpenGL context to render.
/// @brief Is a wrapper for the GLSL shader program code. 
>>>>>>> b50e5ef (Add some implementation files)
class Shader
{
public:
	Shader();
	~Shader();

	/// @brief Initialize the shader. This will compile the GLSL code and update the OpenGL context.
	void activate();

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

<<<<<<< HEAD
		glUseProgram(m_shaderProgramId);
	}

	void unbind()
	{
		glUseProgram(0);
	}

	void updateIntUniform(std::string uniformBindingName, GLint value)
	{
		if (m_shaderProgramId == 0)
		{
			return;
		}

		const GLchar* locationName = uniformBindingName.c_str();
		
		glUniform1i(glGetUniformLocation(m_shaderProgramId, locationName), 0);
	}

	void updateMat4Uniform(std::string uniformBindingName, const glm::mat4& value)
	{
		if (m_shaderProgramId == 0)
		{
			return;
		}

		const GLchar* locationName = uniformBindingName.c_str();
		
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramId, locationName), 1, GL_FALSE, glm::value_ptr(value));

	}

	std::string loadShader(const std::string& fname) 
	{
		std::string result = "";

		std::string line;
		std::ifstream file(fname.c_str());

		if (file.is_open()) 
		{
			while (std::getline(file, line)) 
			{
				result += line + '\n';
			}
		}

		file.close();
		return result;
	}

	unsigned int compileShader(unsigned int type, const std::string& source) 
	{

		unsigned int id;

		if (type == GL_VERTEX_SHADER) 
		{
			id = glCreateShader(GL_VERTEX_SHADER);
		}
		else if (type == GL_FRAGMENT_SHADER) 
		{
			id = glCreateShader(GL_FRAGMENT_SHADER);
		}

		const char* src = source.c_str();

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE) 
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			char* errorMessages = new char[length];
			glGetShaderInfoLog(id, length, &length, errorMessages);

			if (type == GL_VERTEX_SHADER) 
			{
				std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
			}
			else if (type == GL_FRAGMENT_SHADER) 
			{
				std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
			}

			delete[] errorMessages;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}
	unsigned int createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) 
	{
		unsigned int program = glCreateProgram();

		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);
		glValidateProgram(program);

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		int result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);

		if (result == GL_FALSE) 
		{
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* errorMessages = new char[length];
			glGetProgramInfoLog(program, length, &length, errorMessages);
			std::cout << "ERROR: Linking shaders failed: " << errorMessages << "\n";
			delete[] errorMessages;
			return 0;
		}

		return program;
	}
=======
	/// @brief Set a uniform with the given name and 4D matrix value
	/// @param uniformBindingName 
	/// @param value 
	void updateMat4Uniform(const std::string& uniformBindingName, const glm::mat4& value);
>>>>>>> b50e5ef (Add some implementation files)
private:
	std::string loadShader(const std::string& fname);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	GLuint m_shaderProgramId = 0; //Defaults to "no shader"
};
#endif