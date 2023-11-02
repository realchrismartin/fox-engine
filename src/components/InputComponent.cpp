#include "InputComponent.hpp"


void InputComponent::informOfWindowEvent(sf::Event e)
{
	if (e.type != sf::Event::KeyPressed && e.type != sf::Event::KeyReleased)
	{
		return;
	}

	switch (e.type)
	{
		case(sf::Event::KeyPressed):
		{
			switch (e.key.code)
			{
				case(sf::Keyboard::W):
				{
					m_activeInputs.insert(UserInputActionsEnum::PRESSING_W);
					break;
				}
				case(sf::Keyboard::S):
				{

					m_activeInputs.insert(UserInputActionsEnum::PRESSING_S);
					break;
				}
				case(sf::Keyboard::A):
				{
					m_activeInputs.insert(UserInputActionsEnum::PRESSING_A);
					break;
				}
				case(sf::Keyboard::D):
				{
					m_activeInputs.insert(UserInputActionsEnum::PRESSING_D);
					break;
				}
				default:
				{
					//Other keys unhandled for now
					break;
				}
			}
			break;
		}
		case(sf::Event::KeyReleased):
		{
			switch (e.key.code)
			{
				case(sf::Keyboard::W):
				{
					m_activeInputs.erase(UserInputActionsEnum::PRESSING_W);
					break;
				}
				case(sf::Keyboard::S):
				{

					m_activeInputs.erase(UserInputActionsEnum::PRESSING_S);
					break;
				}
				case(sf::Keyboard::A):
				{
					m_activeInputs.erase(UserInputActionsEnum::PRESSING_A);
					break;
				}
				case(sf::Keyboard::D):
				{
					m_activeInputs.erase(UserInputActionsEnum::PRESSING_D);
					break;
				}
				default:
				{
					//Other keys unhandled for now
					break;
				}
			}

			break;
		}
		default:
		{
			//We only care about press and release events. Just return here.
			break;
		}
	}
}

const std::set<UserInputActionsEnum>& InputComponent::getActiveInputs() const
{
	return m_activeInputs;
}
