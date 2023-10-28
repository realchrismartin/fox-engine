#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>

class Shader
{
public:

	GLuint getShaderProgramId()
	{
		return m_shaderProgramId;
	}

	void activateShader()
	{
		std::string vertexShader = loadShader("../../shaders/vert.glsl");
		std::string fragmentShader = loadShader("../../shaders/frag.glsl");
		m_shaderProgramId = createShader(vertexShader, fragmentShader);

		if (m_shaderProgramId == 0)
		{
			return;
		}

		glUseProgram(m_shaderProgramId);
	}

	void deactivateShader()
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

	std::string loadShader(const std::string& fname) 
	{
		std::string result = "";

		std::string line;
		std::ifstream file(fname.c_str());

		if (file.is_open()) 
		{
			while (getline(file, line)) 
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
private:
	GLuint m_shaderProgramId = 0; //Defaults to "no shader"
};
#endif