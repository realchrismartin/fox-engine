#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SFML/Window/Window.hpp"
#include "SFML/Window/Event.hpp"
#include "src/graphics/Shader.hpp"
#include "src/graphics/Texture.hpp"

class GraphicsComponent;
struct Vertex;

/// @brief A nice wrapper for a sf::RenderWindow that provides clean interfaces to Systems that need Window access.
class Window
{
public:

	Window()
	{
		m_renderWindow = std::make_unique<sf::Window>(sf::VideoMode(1600, 1200), "Chris and J's Game", sf::Style::Default, sf::ContextSettings(24,8, 0, 4, 6));
		setupOpenGL();
	}

	void clear();
	void display();
	void pollForEvents();
	bool isOpen() const;
	Shader& getBoundShader();

	const std::vector<sf::Event>& getEvents() const;

	/*
	template<typename T>
	void draw(T drawableComponent)
	{
		draw(drawableComponent.getVertexCount(), drawableComponent.getIndexCount(), drawableComponent.getVertices(), drawableComponent.getIndices());
	}
	*/

	void draw(size_t vertexCount, size_t indexCount, GLvoid* vertices, const std::vector<GLuint>& indices);
private:

	void setupOpenGL();

	std::unique_ptr<sf::Window> m_renderWindow;
	std::vector<sf::Event> m_events;

	Texture m_texture;
	Shader m_shader;

	GLuint m_VAOId;
	GLuint m_vertexBufferObject;
	GLuint m_elementArrayBufferObject;

	GLuint m_maxVertices;
	GLuint m_maxIndices;
};
#endif

