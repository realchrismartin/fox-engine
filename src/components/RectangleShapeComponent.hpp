#ifndef RECTANGLESHAPEOMPONENT_HPP
#define RECTANGLESHAPECOMPONENT_HPP

class PhysicsComponent;

#include "SFML/Graphics/RectangleShape.hpp"

/// @brief A simple graphical component that renders a Rectangle wherever the entity's physics body is.
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
