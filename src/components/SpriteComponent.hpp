#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

class PhysicsComponent;

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

/// @brief A simple graphical Sprite component that renders a Sprite wherever the entity's physics body is
class SpriteComponent
{
public:
	const sf::Sprite& getDrawable() const;
	void setTexturePath(std::string spriteTexturePath);
	void updateWithPhysics(const PhysicsComponent& component);
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
};

#endif
