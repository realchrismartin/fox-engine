#include "src/components/SpriteComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

void SpriteComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//Assume the window is 1600 x 1200
	float windowHeight = 1200.f;
	float windowWidth = 1600.f;

	//Wherever the physics body is now, update the graphic to match.

	//TODO: make this not completely suck. Use matrices, you fool!!
	sf::Vector2f size = component.getSize();
	sf::Vector2f centerPos = component.getCenterPoint();

	sf::Vector2f normalizedSize = sf::Vector2f(size.x / windowWidth, size.y / windowHeight);
	sf::Vector2f normalizedCenterPos = sf::Vector2f(centerPos.x / windowWidth, (windowHeight - centerPos.y) / windowHeight); //Subtraction is to flip y axis
	sf::Vector2f normalizedHalfSize = normalizedSize /= 2.f;

	m_vertices = {
		normalizedCenterPos.x + normalizedHalfSize.x, normalizedCenterPos.y + normalizedHalfSize.y, 0.f, 1.f, 1.f,
		normalizedCenterPos.x + normalizedHalfSize.x, normalizedCenterPos.y - normalizedHalfSize.y, 0.f, 1.f, 0.f,
		normalizedCenterPos.x - normalizedHalfSize.x, normalizedCenterPos.y - normalizedHalfSize.y, 0.f, 0.f, 0.f,
		normalizedCenterPos.x - normalizedHalfSize.x, normalizedCenterPos.y + normalizedHalfSize.y, 0.f, 0.f, 1.f

	};

	m_indices = { 0,1,3 , 1, 2, 3 };
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
