#include "src/components/GraphicsComponent.hpp"

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

void GraphicsComponent::updateWithTransform(TransformComponent& component)
{
	m_shape.setPosition(component.getPosition());
}

void GraphicsComponent::draw(std::shared_ptr<sf::RenderWindow> window) const
{
	window->draw(m_shape);
}

