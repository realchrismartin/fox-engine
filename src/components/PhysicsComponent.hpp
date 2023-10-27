#ifndef PHYSICSCOMPONENT_HPP
#define PHYSICSCOMPONENT_HPP

struct Overlap
{
	sf::Vector2f position = sf::Vector2f(0.f, 0.f); //Top left of the overlap rect
	sf::Vector2f size = sf::Vector2f(0.f, 0.f); //Size of the overlap rect
};

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
	void setHasStaticBody(bool isStatic);
	bool hasStaticBody() const;
	void setStartingPosition(sf::Vector2f position);
private:
	std::optional<Overlap> getOverlap(const PhysicsComponent& otherPhysicsComponent);
	sf::Vector2f m_position = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_size = sf::Vector2f(10.f, 10.f);
	sf::Vector2f m_force = sf::Vector2f(0.f, 0.f);
	float m_mass = 1.f;
	bool m_staticBody = true;
	bool m_touchingStaticBody = false;
};

#endif