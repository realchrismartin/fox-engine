#include "src/components/SpriteComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

void SpriteComponent::setSizeAndCenterPoint(sf::Vector2f size, sf::Vector2f centerPos)
{
	//m_textureCoordinates = sf::Vector2(50, 0);

	//Assume the window is 1600 x 1200
	//TODO: don't assume this, you peon!
	float windowHeight = 1200.f;
	float windowWidth = 1600.f;

	//Upper left of where we want to start the texture
	//TODO: calculate this once and store it.
	sf::Vector2f normalizedTextureCoordinates = sf::Vector2f((float)m_textureCoordinates.x / m_textureWidth, (float)m_textureCoordinates.y / m_textureHeight);
	sf::Vector2f normalizedSpriteSize = sf::Vector2f(m_spriteWidth / m_textureWidth, m_spriteHeight / m_textureHeight); //Assumes texture is bigger than the sprite...

	//TODO: make this not completely suck. Use matrices, you fool!! This sucks!
	sf::Vector2f normalizedSize = sf::Vector2f(size.x / windowWidth, size.y / windowHeight);
	sf::Vector2f normalizedCenterPos = sf::Vector2f(centerPos.x / windowWidth, (windowHeight - centerPos.y) / windowHeight); //Subtraction is to flip y axis
	sf::Vector2f normalizedHalfSize = normalizedSize /= 2.f;

	//Create the equivalent of two triangles taped together (a thing commonly known as a rectangle)
	m_vertices = {
		normalizedCenterPos.x + normalizedHalfSize.x, normalizedCenterPos.y + normalizedHalfSize.y, 0.f, normalizedTextureCoordinates.x + normalizedSpriteSize.x, normalizedTextureCoordinates.y - normalizedSpriteSize.y,
		normalizedCenterPos.x + normalizedHalfSize.x, normalizedCenterPos.y - normalizedHalfSize.y, 0.f, normalizedTextureCoordinates.x + normalizedSpriteSize.x, normalizedTextureCoordinates.y,
		normalizedCenterPos.x - normalizedHalfSize.x, normalizedCenterPos.y - normalizedHalfSize.y, 0.f, normalizedTextureCoordinates.x, normalizedTextureCoordinates.y,
		normalizedCenterPos.x - normalizedHalfSize.x, normalizedCenterPos.y + normalizedHalfSize.y, 0.f, normalizedTextureCoordinates.x,normalizedTextureCoordinates.y - normalizedSpriteSize.y 
	};

	m_indices = { 0,1,3 , 1, 2, 3 };
}

void SpriteComponent::updateWithPhysics(const PhysicsComponent& component)
{
	setSizeAndCenterPoint(component.getSize(), component.getCenterPoint());
}

const std::vector<GLfloat>& SpriteComponent::getVertices() const
{
	return m_vertices;
}

const std::vector<GLuint>& SpriteComponent::getIndices() const
{
	return m_indices;
}

size_t SpriteComponent::getVertexCount() const
{
	return m_vertices.size();
}

size_t SpriteComponent::getIndexCount() const
{
	return m_indices.size();
}
