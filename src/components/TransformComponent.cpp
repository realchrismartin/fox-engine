#include "src/components/TransformComponent.hpp"

void TransformComponent::updateWithInput(InputComponent* inputComponent)
{
	std::vector<UserInputActionsEnum> userInputs =  inputComponent->getActionsSinceLastUpdate();
	
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

	m_position = m_position + addition;
}

sf::Vector2f TransformComponent::getPosition() const
{
	return m_position;
}

void TransformComponent::setPosition(sf::Vector2f position)
{
	m_position = position;
}
