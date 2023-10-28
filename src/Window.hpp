#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "src/graphics/Shader.hpp"
#include "src/graphics/Texture.hpp"

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

	void clear();
	void display();
	void pollForEvents();
	bool isOpen() const;

	const std::vector<sf::Event>& getEvents() const;

	template<typename T>
	void draw(T drawableComponent)
	{
		draw(drawableComponent.getVertexCount(), drawableComponent.getIndexCount(), drawableComponent.getVertices(), drawableComponent.getIndices());
	}

	void draw(size_t vertexCount, size_t indexCount, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
private:

	void setupOpenGL();

	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::vector<sf::Event> m_events;

	Texture texture;
	Shader shader;

	GLuint m_VAOId;
	GLuint m_vertexBufferObject;
	GLuint m_elementArrayBufferObject;

	GLuint m_maxVertices;
	GLuint m_maxIndices;
};
#endif

