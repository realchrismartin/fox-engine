#ifndef PHYSICSCOMPONENT_HPP
#define PHYSICSCOMPONENT_HPP

class InputComponent;

class PhysicsComponent
{
public:
	void updateWithInput(const InputComponent& inputComponent);
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f position);
private:
	sf::Vector2f m_position = sf::Vector2f(0.f,0.f);
};

#endif