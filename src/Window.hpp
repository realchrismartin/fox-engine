#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "src/extern/stb_image.h"
#include "src/graphics/Shader.hpp"

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

		setupOpenGL();
	}
	~Window()
	{
		if (m_textureID < 0)
		{
			glDeleteTextures(1, &m_textureID);
		}
	}

	template<typename T>
	void draw(T drawableComponent)
	{
		if (m_glActive)
		{
			return;
		}
		m_renderWindow->draw(drawableComponent.getDrawable());
	}

	template<typename T>
	void drawGL(T drawableComponent)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		
		glBufferData(GL_ARRAY_BUFFER, drawableComponent.getVertexCount() * sizeof(GLfloat), &drawableComponent.getVertices()[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawableComponent.getIndexCount() * sizeof(GLuint), &drawableComponent.getIndices()[0], GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES,drawableComponent.getIndexCount(), GL_UNSIGNED_INT, nullptr);
	}

	void clear();
	void display();
	void pollForEvents();
	bool isOpen() const;
	const std::vector<sf::Event>& getEvents() const;

private:

	void setupOpenGL()
	{
		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			std::cout << glewGetErrorString(err) << std::endl;
			return;
		}

		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

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

		// Set texture parameters 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* imageData = stbi_load("../../img/character.png", &width, &height, &nrChannels, 0);

		// Load texture data into the bound texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// Unbind the texture once it is loaded, IF we plan on rebindign just before draw
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(imageData);

		shader.activateShader();
		shader.updateIntUniform("textureSampler", 0);

		m_glActive = true;

	}

	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::vector<sf::Event> m_events;

	Shader shader;

	bool m_glActive = false;
	GLuint m_VAOId;
	GLuint m_vertexBufferObject;
	GLuint m_elementArrayBufferObject;
	GLuint m_textureID = 0;

};
#endif

