#ifndef GRAPHICSCOMPONENT_HPP
#define GRAPHICSCOMPONENT_HPP

#include "src/Component.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "src/TransformComponent.hpp"

class GraphicsComponent : public Component
{
public:
	GraphicsComponent();
	virtual void update() override;
	void updateWithTransform(TransformComponent& component);
	void draw(std::shared_ptr<sf::RenderWindow> window) const;
private:
	sf::RectangleShape m_shape;
};

#endif
