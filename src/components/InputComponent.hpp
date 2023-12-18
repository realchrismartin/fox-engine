#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include "extern\sdl\include\SDL3\SDL_events.h"

enum class UserInputActionsEnum
{
	PRESSING_W,
	PRESSING_S,
	PRESSING_A,
	PRESSING_D,
	NONE
};

/// @brief A component which stores events coming from sf::RenderWindow and filters out the ones we don't care about. In the end, it tracks the events that are actively occurring that are "input" related
class InputComponent
{
public:
	glm::vec3 getTranslationGivenInput();
	glm::vec3 getRotationGivenInput();
	bool anyInputActive() const;
	void informOfEvent(const SDL_Event& event);
private:

	static UserInputActionsEnum getInputActionForScancode(SDL_Scancode code);
	void markInputState(UserInputActionsEnum action, bool pressed);

	std::set<UserInputActionsEnum> m_activeInputs;
	float m_translationFactor = .25f; //TODO
};

#endif