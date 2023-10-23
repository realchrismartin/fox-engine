#ifndef GRAPHICSCOMPONENT_HPP
#define GRAPHICSCOMPONENT_HPP

class PhysicsComponent;

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

class GraphicsComponent
{
public:
	void updateWithPhysics(const PhysicsComponent& component);
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void setShape(sf::Color color);
	void setSprite(std::string spriteTexturePath);

	bool hasShape() const;
	bool hasSprite() const;

	const sf::RectangleShape& getShape() const;
	const sf::Sprite& getSprite() const;
private:
	std::shared_ptr<sf::RectangleShape> m_shape;
	std::shared_ptr<sf::Texture> m_texture; //TODO: share a texture across components, this is expensive!
	std::shared_ptr<sf::Sprite> m_sprite;
};

#endif
