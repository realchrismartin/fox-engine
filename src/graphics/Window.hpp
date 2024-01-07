#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "src/graphics/Shader.hpp"
#include "src/graphics/Texture.hpp"
#include "sdl3/SDL.h"

class GraphicsComponent;
struct Vertex;

/// @brief A nice wrapper for a SDL OpenGL window that provides clean interfaces to Systems that need Window access.
class Window
{
public:

	Window();
	~Window();

	/// @brief GLClear the window.
	void clear();

	/// @brief Display whatever's on the backbuffer by flopping the buffers
	void display();

	void draw(size_t vertexCount, size_t indexCount, size_t matrixCount, GLvoid* vertices, GLvoid* indices, GLvoid* mvpMatrices);

	/// @brief Mark the window for closing. No more rendering will be done.
	void close();

	/// @brief Get whether the window is open.
	/// @return 
	bool isOpen() const;

	/// @brief Get the current window size in pixels
	/// @return 
	const glm::i64vec2& getWindowSize() const;

	/// @brief Called when the SDL window events indicate the user resized the window
	void markWindowSizeDirty();

	/// @brief Indicates whether the window was resized since the last update
	/// @return 
	bool isWindowSizeDirty() const;

	/// @brief Mark the window size clean
	void markWindowSizeClean();

private:
	void setupOpenGL();

	SDL_Window* m_window = NULL;

	Texture m_texture;
	Shader m_shader;

	GLuint m_VAOId = 0; //Not correct. Need to set it.
	GLuint m_vertexBufferObject = 1;
	GLuint m_elementArrayBufferObject = 2;
	GLuint m_shaderStorageBufferObject = 3;

	size_t m_maxVerticesPerRender = 0;
	size_t m_maxIndicesPerRender = 0;
	size_t m_maxSSBOMatricesPerRender = 0;

	bool m_open = true;

	glm::i64vec2 m_windowSize = glm::i64vec2(1024, 768);

	bool m_windowSizeDirty = true;
};
#endif

