#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include <fstream>
#include "src/extern/stb_image.h"

class GraphicsComponent;

/// @brief A nice wrapper for a sf::RenderWindow that provides clean interfaces to Systems that need Window access.
class Window
{
public:

	Window()
	{
		m_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1600, 1200), "Chris and J's Game");
		m_renderWindow->setFramerateLimit(60);

		//Make a "camera" that's a view of the window.
		sf::View camera = sf::View(sf::Vector2f(800.f,600.f), sf::Vector2f(1600.f,1200.f));
		//camera.rotate(50.f);
		m_renderWindow->setView(camera);

		//setupOpenGL();
	}
	~Window()
	{
		// Clean up
		//TODO!
		glDeleteTextures(1, &m_textureID);
	}

	template<typename T>
	void draw(T drawableComponent)
	{
		m_renderWindow->draw(drawableComponent.getDrawable());
		//testDrawGL();
	}

	void clear();
	void display();
	void pollForEvents();
	bool isOpen() const;
	const std::vector<sf::Event>& getEvents() const;

private:
	void testDrawGL()
	{
		std::vector<GLfloat> vertices = { 
				 0.5f,  0.5f, 0.0f,   1.f, 1.f,   // top right
				 0.5f, -0.5f, 0.0f,    1.f,0.f,   // bottom right
				-0.5f, -0.5f, 0.0f,    0.f,0.f,   // bottom left
				-0.5f,  0.5f, 0.0f,    0.f,1.f    // top left 
		};

		std::vector<GLuint> indices = { 0,1,3,1,2,3 };

		//TODO: we may want to bind texture just before draw instead of keeping it bound
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void setupOpenGL()
	{
		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			std::cout << glewGetErrorString(err) << std::endl;
			return;
		}

		std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

		//Create shader program
		std::string vertexShader = loadShader("../../shaders/vert.glsl");
		std::string fragmentShader = loadShader("../../shaders/frag.glsl");
		unsigned int shader = createShader(vertexShader, fragmentShader);
		glUseProgram(shader);

		//Create buffer objects
		glGenVertexArrays(1, &m_VAOId);
		glBindVertexArray(m_VAOId);

		//Create and init VBO
		glGenBuffers(1, &m_vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

		//Create and init EBO
		glGenBuffers(2, &m_elementArrayBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBufferObject);

		GLint vertexPosLocation = 0;
		GLint texCoordLocation = 1;

		glVertexAttribPointer(vertexPosLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glEnableVertexAttribArray(vertexPosLocation);
		glEnableVertexAttribArray(texCoordLocation);

		// Create a texture object
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);

		// Set texture parameters 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* imageData = stbi_load("../../img/character.png", &width, &height, &nrChannels, 0);

		// Load texture data into the bound texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// Unbind the texture once it is loaded, IF we plan on rebindign just before draw
		//glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(imageData);
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

	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::vector<sf::Event> m_events;

	GLuint m_gProgramID; //References to OpenGL objects
	GLuint m_VAOId;
	GLuint m_vertexBufferObject;
	GLuint m_elementArrayBufferObject;
	GLuint m_textureID;

};
#endif

