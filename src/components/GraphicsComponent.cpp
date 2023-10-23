#include "src/components/GraphicsComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

GraphicsComponent::GraphicsComponent()
{
	 sf::RectangleShape shape;
	 shape.setSize(sf::Vector2f(100.f, 100.f));
	 shape.setFillColor(sf::Color::Blue);

	 m_shape = shape;
}

void GraphicsComponent::setColor(sf::Color color)
{
	m_shape.setFillColor(color);
}

void GraphicsComponent::setSize(sf::Vector2f size)
{
	m_shape.setSize(size);
}

void GraphicsComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//Wherever the physics body is now, update the graphic to match.
	setPosition(component.getPosition());
}

void GraphicsComponent::setPosition(sf::Vector2f position)
{
	m_shape.setPosition(position);
}

void GraphicsComponent::draw(sf::RenderWindow& window) const
{
	window.draw(m_shape);
}
