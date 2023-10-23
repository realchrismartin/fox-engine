#include "src/components/RectangleShapeComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

const sf::RectangleShape& RectangleShapeComponent::getDrawable() const
{
	return m_rectangleShape;
}

void RectangleShapeComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//Wherever the physics body is now, update the graphic to match.
	setPosition(component.getPosition());

	//Set the size too
	setSize(component.getSize());
}

void RectangleShapeComponent::setFillColor(const sf::Color color)
{
	m_rectangleShape.setFillColor(color);
}

void RectangleShapeComponent::setPosition(sf::Vector2f position)
{
	m_rectangleShape.setPosition(position);
}

void RectangleShapeComponent::setSize(sf::Vector2f size)
{
	m_rectangleShape.setSize(size);
}

