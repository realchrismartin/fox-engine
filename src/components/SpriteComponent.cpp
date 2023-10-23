#include "src/components/SpriteComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

void SpriteComponent::setTexturePath(std::string spriteTexturePath)
{
	 //TODO: this is $$! Change it.
	 m_texture.loadFromFile(spriteTexturePath);
	 m_sprite.setTexture(m_texture);
}

const sf::Sprite& SpriteComponent::getDrawable() const
{
	return m_sprite;
}

void SpriteComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//TODO: make this cheaper.

	//Wherever the physics body is now, update the graphic to match.
	setPosition(component.getPosition());

	//Set the size too
	setSize(component.getSize());
}

void SpriteComponent::setPosition(sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void SpriteComponent::setSize(sf::Vector2f size)
{
	float scaleX = size.x /m_sprite.getTextureRect().width;
	float scaleY = size.y / m_sprite.getTextureRect().height;

	m_sprite.setScale(sf::Vector2f(scaleX, scaleY));
}

