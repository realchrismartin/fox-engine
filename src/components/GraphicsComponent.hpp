#ifndef GRAPHICSCOMPONENT_HPP
#define GRAPHICSCOMPONENT_HPP

#include "src/components/TransformComponent.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class GraphicsComponent
{
public:
	GraphicsComponent();
	void updateWithTransform(TransformComponent& component);
	void setColor(sf::Color color);
	void draw(std::shared_ptr<sf::RenderWindow> window) const;
private:
	sf::RectangleShape m_shape;
};

#endif
