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
	void clear();
	void display();
	Shader& getBoundShader();

	void draw(size_t vertexCount, size_t indexCount, size_t matrixCount, GLvoid* vertices, GLvoid* indices, GLvoid* mvpMatrices);

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

};
#endif

