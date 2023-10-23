#include "src/components/PhysicsComponent.hpp"

#include "src/components/InputComponent.hpp"
#include "src/util/Utils.hpp"

void PhysicsComponent::updateWithInput(const InputComponent& inputComponent, float elapsedTime)
{
	std::set<UserInputActionsEnum> userInputs =  inputComponent.getActiveInputs();

	//TODO: make this less, uh, arbitrary
	float forceFactor = elapsedTime * .0005f * m_mass;

	for (auto const& enumValue : userInputs)
	{
		switch (enumValue)
		{
			case(UserInputActionsEnum::PRESSING_A):
			{
				m_force += elapsedTime * sf::Vector2f(-1.f * forceFactor,0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSING_D):
			{
				m_force += elapsedTime * sf::Vector2f(forceFactor, 0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSING_SPACEBAR):
			{
				if (m_touchingStaticBody)
				{
					m_force += elapsedTime * sf::Vector2f(0.f,-forceFactor * 10.f);
				}
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

	std::optional<Overlap> overlap = getOverlap(otherPhysicsComponent);

	if (!overlap.has_value())
	{
		//No collision
		return;
	}
	
	//If the thing we're overlapping is static and it's below us, turn gravity off and skip pushing back on that thing.
	if (otherPhysicsComponent.hasStaticBody() && otherPhysicsComponent.getPosition().y > m_position.y)
	{
		m_touchingStaticBody = true;
		return;
	}

	sf::Vector2f overlapCenterPoint = sf::Vector2f(overlap.value().position.x + (overlap.value().size.x * .5f), overlap.value().position.y + (overlap.value().size.y * .5f));
	sf::Vector2f centerPoint = getCenterPoint();
	sf::Vector2f directionVector = centerPoint - overlapCenterPoint;

	Utils::normalize(directionVector);
	//Return the center point of the rectangle created by the intersection

	//If we're already touching a static body below us this tick, remove the Y component of the directional force for this collision so we don't get pooshed through a floor.
	if (m_touchingStaticBody)
	{
		directionVector.y = 0.f;
	}

	//This thing is what is getting moved. Calculate some displacement force based on how much its overlapping based on its mass (rudimentary!)
	//Determine what fraction of the overall mass of this thing is in play.
	float massFactorToApply = m_mass * ((overlap.value().size.x * overlap.value().size.y) / (m_size.x * m_size.y));

	//Apply that force!
	m_force += massFactorToApply * elapsedTime * directionVector;
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

	//Add the force of gravity if not touching something static
	//TODO: this is very wrong.
	if(!m_touchingStaticBody)
	{
		m_force += sf::Vector2f(0.f, .2f * m_mass);
	}

	//Calculate the velocity of the body
	//Velocity = force / mass
	sf::Vector2f velocity = m_force /= m_mass;

	//Distance = velocity * time;
	sf::Vector2f distance = velocity * elapsedTime;

	//Update the position based on the velocity
	m_position += distance;

	//Reset collision check
	m_touchingStaticBody = false;
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

void PhysicsComponent::setHasStaticBody(bool isStatic)
{
	m_staticBody = isStatic;
}

bool PhysicsComponent::hasStaticBody() const
{
	return m_staticBody;
}

//TODO: lose thsi later
void PhysicsComponent::setStartingPosition(sf::Vector2f position)
{
	m_position = position;
}

std::optional<Overlap> PhysicsComponent::getOverlap(const PhysicsComponent& otherPhysicsComponent)
{
	//We assume both components are un-rotated rectangles
	sf::Vector2f topLeft = sf::Vector2f(std::max(m_position.x, otherPhysicsComponent.getPosition().x), std::max(m_position.y, otherPhysicsComponent.getPosition().y));
	sf::Vector2f bottomRight = sf::Vector2f(std::min(m_position.x + m_size.x, otherPhysicsComponent.getPosition().x + otherPhysicsComponent.getSize().x),std::min(m_position.y + m_size.y, otherPhysicsComponent.getPosition().y + otherPhysicsComponent.getSize().y));

	if (topLeft.x >= bottomRight.x || topLeft.y >= bottomRight.y)
	{
		//There is no intersection
		return std::nullopt;
	}
	
	Overlap overlap;
	overlap.position = topLeft;
	overlap.size = sf::Vector2f(bottomRight.x - topLeft.x,bottomRight.y - topLeft.y);

	return overlap;
}
