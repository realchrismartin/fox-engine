#include "src/components/PhysicsComponent.hpp"

#include "src/components/InputComponent.hpp"

void PhysicsComponent::updateWithInput(const InputComponent& inputComponent)
{
	std::vector<UserInputActionsEnum> userInputs =  inputComponent.getActionsSinceLastUpdate();
	
	sf::Vector2f addition = sf::Vector2f(0.f, 0.f);

	for (auto const& enumValue : userInputs)
	{
		switch (enumValue)
		{
			case(UserInputActionsEnum::PRESSED_W):
			{
				addition += sf::Vector2f(0.f, -50.f);
				break;
			}
			case(UserInputActionsEnum::PRESSED_S):
			{
				addition += sf::Vector2f(0.f, 50.f);
				break;
			}
			case(UserInputActionsEnum::PRESSED_A):
			{
				addition += sf::Vector2f(-50.f,0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSED_D):
			{
				addition += sf::Vector2f(50.f, 0.f);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	//TODO: make this velocity or some shit instead of position.
	m_position = m_position + addition;
}

sf::Vector2f PhysicsComponent::getPosition() const
{
	return m_position;
}

void PhysicsComponent::setPosition(sf::Vector2f position)
{
	m_position = position;
}
