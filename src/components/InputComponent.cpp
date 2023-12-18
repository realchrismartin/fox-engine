#include "InputComponent.hpp"

void InputComponent::informOfEvent(const SDL_Event& event)
{
	//TODO: add support for mouse events
	if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP)
	{
		return;
	}

	//Make sure to update boolean here if adding mouse events
	markInputState(getInputActionForScancode(event.key.keysym.scancode), event.type == SDL_EVENT_KEY_DOWN);
}

UserInputActionsEnum InputComponent::getInputActionForScancode(SDL_Scancode code)
{
	switch (code)
	{
		case(SDL_SCANCODE_W):
			return UserInputActionsEnum::PRESSING_W;
		case(SDL_SCANCODE_S):
			return UserInputActionsEnum::PRESSING_S;
		case(SDL_SCANCODE_A):
			return UserInputActionsEnum::PRESSING_A;
		case(SDL_SCANCODE_D):
			return UserInputActionsEnum::PRESSING_D;
		default:
			return UserInputActionsEnum::NONE;
	}

	return UserInputActionsEnum::NONE;
}

void InputComponent::markInputState(UserInputActionsEnum action, bool pressed)
{
	if (pressed)
	{
		m_activeInputs.insert(action);
	}
	else 
	{
		m_activeInputs.erase(action);
	}
}

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
