#include "src/components/GraphicsComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

void GraphicsComponent::setSize(sf::Vector2f size)
{
	if(m_shape)
	{
		m_shape->setSize(size);
	}

	if (m_sprite)
	{
		float scaleX = size.x /m_sprite->getTextureRect().width;
		float scaleY = size.y / m_sprite->getTextureRect().height;
		m_sprite->setScale(sf::Vector2f(scaleX, scaleY));
	}
}

void GraphicsComponent::setShape(sf::Color color)
{
	m_shape = std::make_shared<sf::RectangleShape>();
	m_shape->setFillColor(color);
}

void GraphicsComponent::setSprite(std::string spriteTexturePath)
{
	 //TODO: this is $$! Change it.
	 m_texture = std::make_shared<sf::Texture>();
	 m_texture->loadFromFile("../../img/character.png");

	 m_sprite = std::make_shared<sf::Sprite>();
	 m_sprite->setTexture(*m_texture);
}

bool GraphicsComponent::hasShape() const
{
	if (m_shape)
	{
		return true;
	}

	return false;
}

bool GraphicsComponent::hasSprite() const
{
	if (m_sprite)
	{
		return true;
	}

	return false;

}

const sf::RectangleShape& GraphicsComponent::getShape() const
{
	return *m_shape;
}

const sf::Sprite& GraphicsComponent::getSprite() const
{
	return *m_sprite;
}

void GraphicsComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//Wherever the physics body is now, update the graphic to match.
	setPosition(component.getPosition());
}

void GraphicsComponent::setPosition(sf::Vector2f position)
{
	if (m_shape)
	{
		m_shape->setPosition(position);
	}

	if (m_sprite)
	{
		m_sprite->setPosition(position);
	}
}
