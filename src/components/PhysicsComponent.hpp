#ifndef PHYSICSCOMPONENT_HPP
#define PHYSICSCOMPONENT_HPP

class InputComponent;

class PhysicsComponent
{
public:
	void update(float elapsedTime);
	void updateCollision(const PhysicsComponent& otherPhysicsComponent, float elapsedTime);
	void updateWithInput(const InputComponent& inputComponent, float elapsedTime);

	sf::Vector2f getPosition() const;
	sf::Vector2f getCenterPoint() const;
	sf::Vector2f getSize() const;

	void setSize(sf::Vector2f size);
	void setMass(float mass);
	void setIsStaticBody(bool isStatic);
	void setStartingPosition(sf::Vector2f position);
private:
	std::optional<sf::Vector2f> getCenterPointOfOverlap(const PhysicsComponent& otherPhysicsComponent);
	sf::Vector2f m_position = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_size = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_force = sf::Vector2f(0.f, 0.f);
	float m_mass = 1.f;
	bool m_staticBody = true;
};

#endif