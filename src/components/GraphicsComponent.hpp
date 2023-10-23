#ifndef GRAPHICSCOMPONENT_HPP
#define GRAPHICSCOMPONENT_HPP

class PhysicsComponent;

#include "SFML/Graphics/RectangleShape.hpp"

class GraphicsComponent
{
public:
	GraphicsComponent();
	void updateWithPhysics(const PhysicsComponent& component);
	void setPosition(sf::Vector2f position);
	void setColor(sf::Color color);
	void setSize(sf::Vector2f size);
	const sf::RectangleShape& getGraphic() const;
private:
	sf::RectangleShape m_shape;
};

#endif
