#include "ModelComponent.hpp"

ModelComponent::ModelComponent()
{
	sf::Vector2f normalizedTextureCoordinates = sf::Vector2f((float)m_textureCoordinates.x / m_textureWidth, (float)m_textureCoordinates.y / m_textureHeight);
	sf::Vector2f normalizedSpriteSize = sf::Vector2f(m_spriteWidth / m_textureWidth, m_spriteHeight / m_textureHeight); //Assumes texture is bigger than the sprite...

	m_vertices =
	{
		1.f,1.f,0.f,normalizedTextureCoordinates.x + normalizedSpriteSize.x, normalizedTextureCoordinates.y,
		1.f,-1.f,0.f,normalizedTextureCoordinates.x + normalizedSpriteSize.x, normalizedTextureCoordinates.y - normalizedSpriteSize.y,
		-1.f,-1.f,0.f,normalizedTextureCoordinates.x, normalizedTextureCoordinates.y - normalizedSpriteSize.y,
		-1.f,1.f,0.f,normalizedTextureCoordinates.x,normalizedTextureCoordinates.y
	};

	m_indices = { 0,1,3, 1, 2, 3 };
}

const std::vector<GLfloat>& ModelComponent::getVertices() const
{
	return m_vertices;
}

const std::vector<GLuint>& ModelComponent::getIndices() const
{
	return m_indices;
}
