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

	Window();
	void clear();
	void display();
	void pollForEvents();
	bool isOpen() const;
	Shader& getBoundShader();

	const std::vector<sf::Event>& getEvents() const;

	void draw(size_t vertexCount, size_t indexCount, size_t matrixCount, GLvoid* vertices, GLvoid* indices, GLvoid* mvpMatrices);

private:
	void setupOpenGL();

	std::unique_ptr<sf::Window> m_renderWindow;
	std::vector<sf::Event> m_events;

	Texture m_texture;
	Shader m_shader;

	GLuint m_VAOId;
	GLuint m_vertexBufferObject = 1;
	GLuint m_elementArrayBufferObject = 2;
	GLuint m_shaderStorageBufferObject = 3;

	size_t m_maxVerticesPerRender;
	size_t m_maxIndicesPerRender;
	size_t m_maxSSBOMatricesPerRender;

};
#endif

