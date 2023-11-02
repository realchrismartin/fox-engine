#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

class ModelComponent
{
public:
	ModelComponent();
	const std::vector<GLfloat>& getVertices() const;
	const std::vector<GLuint>& getIndices() const;
private:
	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;

	sf::Vector2i m_textureCoordinates = sf::Vector2i(0,0);

	float m_textureHeight = 100.f;
	float m_textureWidth = 100.f;

	float m_spriteHeight = 50.f;
	float m_spriteWidth = 50.f;

};

#endif