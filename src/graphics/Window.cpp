#include "src/graphics/Window.hpp"
#include "src/graphics/Vertex.hpp"

Window::Window()
{

	m_window = SDL_CreateWindow("FnF", 1600, 1200, SDL_WINDOW_OPENGL);

	if (m_window == NULL)
	{

		SDL_Log("SDL_CreateWindow failed (%s)", SDL_GetError());
		return;
	}

	SDL_SetWindowTitle(m_window, "FnF");


	//Set up the OpenGL context in the window
	setupOpenGL();
}

Window::~Window()
{
	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
	}
}

void Window::setupOpenGL()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//TODO: do we need to keep this variable?
	auto gContext = SDL_GL_CreateContext(m_window);

	if (gContext == NULL)
	{
		SDL_Log("SDL_OpenGL context could not be created  (%s)", SDL_GetError());
		return;
	}

	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		std::cout << glewGetErrorString(err) << std::endl;
		return;
	}

	std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);

	//Create the VAO so we can store properties on it
	glGenVertexArrays(1, &m_VAOId);
	glBindVertexArray(m_VAOId);

	//Create and init VBO
	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

	//Create and init EBO
	glGenBuffers(2, &m_elementArrayBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBufferObject);

	//Create and init SSBO
	glGenBuffers(3, &m_shaderStorageBufferObject);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shaderStorageBufferObject);

	//Store properties on the VAO
	constexpr GLint vertexPosLocation = 0;
	constexpr GLint texCoordLocation = 1;
	constexpr GLint mvpIndexLocation = 2;

	glVertexAttribPointer(vertexPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,s))); 
	glVertexAttribPointer(mvpIndexLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,mvpIndex)));

	glEnableVertexAttribArray(vertexPosLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glEnableVertexAttribArray(mvpIndexLocation);

	m_maxVerticesPerRender = 90000;
	m_maxIndicesPerRender = 90000;
	m_maxSSBOMatricesPerRender = 1000;

	//Initialize the buffers with null data and their max sizes.
	glBufferData(GL_ARRAY_BUFFER,m_maxVerticesPerRender * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_maxIndicesPerRender * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_SHADER_STORAGE_BUFFER,m_maxSSBOMatricesPerRender * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW); 

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_shaderStorageBufferObject);

	m_texture.activate();
	m_texture.bind();

	m_shader.activate();

	m_shader.bind();
	m_shader.updateIntUniform("textureSampler", 0);
	m_shader.unbind();

	//un-bind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Window::draw(size_t vertexCount, size_t indexCount, size_t matrixCount, GLvoid* vertices, GLvoid* indices, GLvoid* mvpMatrices)
{

	if (vertexCount <= (size_t)0 || indexCount <= (size_t)0)
	{
		Logger::log("Either no vertices or indices were sent to render.");
		return;
	}

	if (matrixCount > m_maxSSBOMatricesPerRender)
	{
		Logger::log("Too many matrices to render!");
		return;
	}

	if (vertexCount > m_maxVerticesPerRender)
	{
		Logger::log("Too many vertices to render!");
		return;
	}

	if (indexCount > m_maxIndicesPerRender)
	{
		Logger::log("Too many indices to render!");
		return;
	}

	m_shader.bind();
	m_texture.bind();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBufferObject);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shaderStorageBufferObject);

	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(Vertex) * vertexCount, vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, sizeof(GLuint) * indexCount, indices);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER,0, sizeof(glm::mat4) * matrixCount, mvpMatrices);

	glDrawElements(GL_TRIANGLES,(GLsizei)indexCount, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	m_shader.unbind();
	m_texture.unbind();

}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display()
{
	//Swap buffers so we display what's up.
	SDL_GL_SwapWindow(m_window); 
}

Shader& Window::getBoundShader() 
{
	return m_shader;
}
