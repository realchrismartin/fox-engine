#include "InputComponent.hpp"

void InputComponent::clearEvents()
{
	m_actionSinceLastUpdate.clear();
}

std::vector<UserInputActionsEnum>& InputComponent::getActionsSinceLastUpdate()
{
	return m_actionSinceLastUpdate;
}

void InputComponent::informOfWindowEvent(sf::Event e)
{
	if (e.type != sf::Event::KeyPressed)
	{
		return;
	}

	switch (e.key.code)
	{
		case(sf::Keyboard::W):
		{
			m_actionSinceLastUpdate.push_back(UserInputActionsEnum::PRESSED_W);
			break;
		}
		case(sf::Keyboard::S):
		{
			m_actionSinceLastUpdate.push_back(UserInputActionsEnum::PRESSED_S);
			break;
		}
		case(sf::Keyboard::A):
		{
			m_actionSinceLastUpdate.push_back(UserInputActionsEnum::PRESSED_A);
			break;
		}
		case(sf::Keyboard::D):
		{
			m_actionSinceLastUpdate.push_back(UserInputActionsEnum::PRESSED_D);
			break;
		}
		default:
		{
			break;
		}
	}
}
