#include "src/Window.hpp"

#include "SFML/Graphics/View.hpp"
#include "src/graphics/Vertex.hpp"
#include "src/graphics/MVPMatrix.hpp"

void Window::setupOpenGL()
{
	m_renderWindow->setActive(true);

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

	constexpr GLint vertexPosLocation = 0;
	constexpr GLint texCoordLocation = 1;
	constexpr GLint mvpIndexLocation = 2;

	//Store properties on the VAO
	glVertexAttribPointer(vertexPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat))); //TODO: change offset to be based on member location in Vertex
	glVertexAttribPointer(mvpIndexLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(GLfloat))); //TODO: change offset to be based on member location in Vertex

	glEnableVertexAttribArray(vertexPosLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glEnableVertexAttribArray(mvpIndexLocation);

	//Setup the maxima
	//TODO: something is wrong. This has to be done here or maxVertices gets set to 3??
	m_maxVertices = 99000;
	m_maxIndices = 99000;
	m_maxMatrices = 1000;

	//Initialize the buffers with null data and their max sizes.
	glBufferData(GL_ARRAY_BUFFER,m_maxVertices * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxIndices * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxMatrices * sizeof(MVPMatrix), nullptr, GL_STATIC_DRAW); //TODO: does this want static?

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

	if (vertexCount > m_maxVertices)
	{
		Logger::log("Too many vertices to render!");
		return;
	}

	if (indexCount > m_maxIndices)
	{
		Logger::log("Too many indices to render!");
		return;
	}

	if (matrixCount > m_maxMatrices)
	{
		Logger::log("Too many matrices to render!");
		return;
	}

	m_shader.bind();
	m_texture.bind();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBufferObject);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shaderStorageBufferObject);
	auto ssboAddr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(Vertex) * vertexCount, vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, sizeof(GLuint) * indexCount, indices);
	std::memcpy(ssboAddr, mvpMatrices, sizeof(MVPMatrix) * matrixCount);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glDrawElements(GL_TRIANGLES,(GLsizei)indexCount, GL_UNSIGNED_INT, nullptr);
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display()
{
	m_renderWindow->display();
}

void Window::pollForEvents()
{
	m_events.clear();

	sf::Event event;
	while (m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			//Fuck it, we're just gonna close up shop here.
			m_renderWindow->close();
		}

		m_events.push_back(event);
	}
}

bool Window::isOpen() const
{
	return m_renderWindow->isOpen();
}

Shader& Window::getBoundShader() 
{
	return m_shader;
}

const std::vector<sf::Event>& Window::getEvents() const
{
	return m_events;
}
