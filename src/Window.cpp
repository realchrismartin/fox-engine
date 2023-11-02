#include "src/Window.hpp"
#include "SFML/Graphics/View.hpp"

void Window::setupOpenGL()
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

	//Setup the maxima
	//TODO: something is wrong. This has to be done here or maxVertices gets set to 3??
	m_maxVertices = 100;
	m_maxIndices = 100;

	//Initialize the buffers with null data and their max sizes.
	glBufferData(GL_ARRAY_BUFFER,m_maxVertices * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxIndices * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

	m_texture.activate();
	m_texture.bind();

	m_shader.activate();

	m_shader.bind();
	m_shader.updateIntUniform("textureSampler", 0);
	m_shader.unbind();
}

void Window::draw(size_t vertexCount, size_t indexCount, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
{
	if (vertexCount <= (size_t)0 || indexCount <= (size_t)0)
	{
		return;
	}

	m_shader.bind();
	m_texture.bind();

	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(GLfloat) * vertexCount, &vertices[0]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, sizeof(GLuint) * indexCount, &indices[0]);

	glDrawElements(GL_TRIANGLES,indexCount, GL_UNSIGNED_INT, nullptr);
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
