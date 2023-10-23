#ifndef RECTANGLESHAPEOMPONENT_HPP
#define RECTANGLESHAPECOMPONENT_HPP

class PhysicsComponent;

#include "SFML/Graphics/RectangleShape.hpp"

class RectangleShapeComponent
{
public:
	const sf::RectangleShape& getDrawable() const;
	void updateWithPhysics(const PhysicsComponent& component);
	void setFillColor(const sf::Color color);
	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f position);
private:
	sf::RectangleShape m_rectangleShape;
};

#endif
