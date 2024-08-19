#include "InputComponent.hpp"

void InputComponent::onEvent(const InputEvent& inputEvent)
{
	if (!inputEvent.inputEvent.has_value())
	{
		return;
	}

	SDL_Event event = inputEvent.inputEvent.value();

	if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
	{
		markKeyInputState(getInputActionForScancode(event.key.keysym.scancode), event.type == SDL_EVENT_KEY_DOWN);
		return;
	}

	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		glm::vec2 mouseCoordinates = { event.button.x,event.button.y };

		if (event.button.button == SDL_BUTTON_LEFT)
		{
			markMouseInputState(UserInputActionsEnum::LEFT_CLICKING, event.type == SDL_EVENT_MOUSE_BUTTON_DOWN, mouseCoordinates);
		}
		else if (event.button.button == SDL_BUTTON_RIGHT)
		{
			markMouseInputState(UserInputActionsEnum::RIGHT_CLICKING, event.type == SDL_EVENT_MOUSE_BUTTON_DOWN, mouseCoordinates);
		}
	}
}

std::optional<glm::vec2> InputComponent::getInputActionWindowCoordinates(UserInputActionsEnum action) const
{
	if (!m_actionCoordinates.count(action))
	{
		return std::nullopt;
	}
	
	return m_actionCoordinates.at(action);
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

void InputComponent::markKeyInputState(UserInputActionsEnum action, bool pressed)
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

void InputComponent::markMouseInputState(UserInputActionsEnum action, bool pressed, const glm::vec2& coordinates)
{
	if (pressed)
	{
		m_activeInputs.insert(action);
		m_actionCoordinates.emplace(action, coordinates);
	}
	else 
	{
		m_activeInputs.erase(action);
		m_actionCoordinates.erase(action);
	}
}

bool InputComponent::anyInputActive() const
{
	return !m_activeInputs.empty();
}

const std::set<UserInputActionsEnum>& InputComponent::getActiveInputs() const
{
	return m_activeInputs;
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


