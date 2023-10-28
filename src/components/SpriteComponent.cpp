#include "src/components/SpriteComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

void SpriteComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//Assume the window is 1600 x 1200
	//TODO: don't assume this, you peon!
	float windowHeight = 1200.f;
	float windowWidth = 1600.f;

	//Wherever the physics body is now, update the graphic to match.
	sf::Vector2f size = component.getSize(); 
	sf::Vector2f centerPos = component.getCenterPoint();

	//TODO: make these actual coordinates on the sprite sheet based on the texture size
	//We probably need to know how big the sprite is vs the size of the thing in order to do this properly.
	sf::Vector2f spriteBottomLeft = sf::Vector2f(0.f, 0.f);
	sf::Vector2f spriteUpperRight = sf::Vector2f(1.f, 1.f); //1 is "the entire width/height of the texture" ie the entire sprite sheet.

	//TODO: make this not completely suck. Use matrices, you fool!! This sucks!
	sf::Vector2f normalizedSize = sf::Vector2f(size.x / windowWidth, size.y / windowHeight);
	sf::Vector2f normalizedCenterPos = sf::Vector2f(centerPos.x / windowWidth, (windowHeight - centerPos.y) / windowHeight); //Subtraction is to flip y axis
	sf::Vector2f normalizedHalfSize = normalizedSize /= 2.f;

	//Create the equivalent of two triangles taped together (a thing commonly known as a rectangle)
	m_vertices = {
		normalizedCenterPos.x + normalizedHalfSize.x, normalizedCenterPos.y + normalizedHalfSize.y, 0.f, spriteUpperRight.x, spriteUpperRight.y,
		normalizedCenterPos.x + normalizedHalfSize.x, normalizedCenterPos.y - normalizedHalfSize.y, 0.f, spriteUpperRight.x, spriteBottomLeft.y,
		normalizedCenterPos.x - normalizedHalfSize.x, normalizedCenterPos.y - normalizedHalfSize.y, 0.f, spriteBottomLeft.x, spriteBottomLeft.y,
		normalizedCenterPos.x - normalizedHalfSize.x, normalizedCenterPos.y + normalizedHalfSize.y, 0.f, spriteBottomLeft.x,spriteUpperRight.y 
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
