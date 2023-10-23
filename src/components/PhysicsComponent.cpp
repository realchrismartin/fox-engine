#include "src/components/PhysicsComponent.hpp"

#include "src/components/InputComponent.hpp"
#include "src/util/Utils.hpp"

void PhysicsComponent::updateWithInput(const InputComponent& inputComponent, float elapsedTime)
{
	std::vector<UserInputActionsEnum> userInputs =  inputComponent.getActionsSinceLastUpdate();
	
	for (auto const& enumValue : userInputs)
	{
		switch (enumValue)
		{
			case(UserInputActionsEnum::PRESSED_W):
			{
				m_force += elapsedTime * sf::Vector2f(0.f, -.1f);
				break;
			}
			case(UserInputActionsEnum::PRESSED_S):
			{
				m_force += elapsedTime * sf::Vector2f(0.f, .1f);
				break;
			}
			case(UserInputActionsEnum::PRESSED_A):
			{
				m_force += elapsedTime * sf::Vector2f(-.1f,0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSED_D):
			{
				m_force += elapsedTime * sf::Vector2f(.1f, 0.f);
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void PhysicsComponent::updateCollision(const PhysicsComponent& otherPhysicsComponent, float elapsedTime)
{
	if (m_staticBody)
	{
		return;
	}

	std::optional<sf::Vector2f> centerOfOverlap = getCenterPointOfOverlap(otherPhysicsComponent);

	if (!centerOfOverlap.has_value())
	{
		//No collision
		return;
	}

	//TODO: there's something wrong with this calculation - the x value should be 0
	sf::Vector2f directionVector = centerOfOverlap.value() - getCenterPoint();
	Utils::normalize(directionVector);

	//TODO: calculate some kind of mass value to derive how much force to apply
	m_force += elapsedTime * directionVector;
}

void PhysicsComponent::update(float elapsedTime)
{
	if (m_mass <= 0.f)
	{
		Logger::log("This physics body has zero mass. Not updating it!");
		return;
	}

	if (m_staticBody)
	{
		return; //Doesn't update its position, etc
	}

	//Add the force of gravity
	//TODO
	//m_force += sf::Vector2f(0.f, 9.8f);

	//Calculate the velocity of the body
	//Velocity = force / mass
	sf::Vector2f velocity = m_force /= m_mass;

	//Distance = velocity * time;
	sf::Vector2f distance = velocity * elapsedTime;

	//Update the position based on the velocity
	m_position += distance;

	//Reset the force that was applied so it can be accumulated next tick.
	m_force = sf::Vector2f(0.f, 0.f);
}

sf::Vector2f PhysicsComponent::getSize() const
{
	return m_size;
}

sf::Vector2f PhysicsComponent::getPosition() const
{
	return m_position;
}

sf::Vector2f PhysicsComponent::getCenterPoint() const
{
	return sf::Vector2f(m_position.x + m_size.x * .5f, m_position.y + m_size.y * .5f);
}

void PhysicsComponent::setSize(sf::Vector2f size)
{
	m_size = size;
}

void PhysicsComponent::setMass(float mass)
{
	m_mass = mass;
}

void PhysicsComponent::setIsStaticBody(bool isStatic)
{
	m_staticBody = isStatic;
}

//TODO: lose thsi later
void PhysicsComponent::setStartingPosition(sf::Vector2f position)
{
	m_position = position;
}

std::optional<sf::Vector2f> PhysicsComponent::getCenterPointOfOverlap(const PhysicsComponent& otherPhysicsComponent)
{
	//We assume both components are un-rotated rectangles
	sf::Vector2f topLeft = sf::Vector2f(std::max(m_position.x, otherPhysicsComponent.getPosition().x), std::max(m_position.y, otherPhysicsComponent.getPosition().y));
	sf::Vector2f bottomRight = sf::Vector2f(std::min(m_position.x + m_size.x, otherPhysicsComponent.getPosition().x + otherPhysicsComponent.getSize().x),std::min(m_position.y + m_size.y, otherPhysicsComponent.getPosition().y + otherPhysicsComponent.getSize().y));

	if (topLeft.x >= bottomRight.x || topLeft.y >= bottomRight.y)
	{
		//There is no intersection
		return std::nullopt;
	}

	//Return the center point of the rectangle created by the intersection
	return sf::Vector2f((bottomRight.x - topLeft.x) * .5f, (bottomRight.y - topLeft.y) * .5f);
}
