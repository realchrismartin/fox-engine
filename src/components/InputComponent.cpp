#include "InputComponent.hpp"

/*
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
*/

glm::vec3 InputComponent::getTranslationGivenInput()
{
	glm::vec3 vector = glm::vec3(0.f, 0.f, 0.f);

	for (auto const& enumValue : m_activeInputs)
	{
		switch (enumValue)
		{
			case(UserInputActionsEnum::PRESSING_W):
			{
				vector += glm::vec3(0.f, 0.f, m_translationFactor);
				break;
			}
			case(UserInputActionsEnum::PRESSING_S):
			{
				vector += glm::vec3(0.f,0.f,-1.f * m_translationFactor);
				break;
			}
			case(UserInputActionsEnum::PRESSING_A):
			{
				vector += glm::vec3(m_translationFactor, 0.f, 0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSING_D):
			{
				vector += glm::vec3(-1.f * m_translationFactor, 0.f, 0.f);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	return vector;
}

glm::vec3 InputComponent::getRotationGivenInput()
{
	glm::vec3 vector = glm::vec3(0.f, 0.f, 0.f);

	for (auto const& enumValue : m_activeInputs)
	{
		switch (enumValue)
		{
			case(UserInputActionsEnum::PRESSING_W):
			{
				vector = glm::vec3(0.f, 270.f, 0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSING_S):
			{
				vector = glm::vec3(0.f, 90.f, 0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSING_A):
			{
				vector = glm::vec3(0.f, 0.f, 0.f);
				break;
			}
			case(UserInputActionsEnum::PRESSING_D):
			{
				vector = glm::vec3(0.f, 180.f, 0.f);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	return vector;
}


bool InputComponent::anyInputActive() const
{
	return !m_activeInputs.empty();
}

