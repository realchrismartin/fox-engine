#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "src/systems/Subscriber.hpp"

class GraphicsComponent;
struct Vertex;

/// @brief A nice wrapper for a SDL OpenGL window that provides clean interfaces to Systems that need Window access.
class Window : public Subscriber<WindowEvent>
{
public:

	Window();
	~Window();

	void onEvent(const WindowEvent& event) override;

	/// @brief GLClear the window.
	void clear();

	/// @brief Display whatever's on the backbuffer by flopping the buffers
	void display();

	void draw(size_t vertexCount, size_t indexCount, size_t matrixCount, GLvoid* vertices, GLvoid* indices, GLvoid* mvpMatrices);

	/// @brief Get whether the window is open.
	/// @return 
	bool isOpen() const;

	static const glm::i32vec2 DEFAULT_WINDOW_SIZE;

private:
	/// @brief Mark the window for closing. No more rendering will be done.
	void close();

	void setupOpenGL();

	SDL_Window* m_window = NULL;

	GLuint m_VAOId = 0; //Not correct. Need to set it.
	GLuint m_vertexBufferObject = 1;
	GLuint m_elementArrayBufferObject = 2;
	GLuint m_shaderStorageBufferObject = 3;

	size_t m_maxVerticesPerRender = 0;
	size_t m_maxIndicesPerRender = 0;
	size_t m_maxSSBOMatricesPerRender = 0;

	bool m_open = true;
};
#endif

