#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include "SFML/Window/Event.hpp"

enum class UserInputActionsEnum
{
	PRESSING_A,
	PRESSING_D,
	PRESSING_SPACEBAR
};

/// @brief A component which stores events coming from sf::RenderWindow and filters out the ones we don't care about. In the end, it tracks the events that are actively occurring that are "input" related
class InputComponent
{
public:
	void informOfWindowEvent(sf::Event event);
	const std::set<UserInputActionsEnum>& getActiveInputs() const;
private:
	std::set<UserInputActionsEnum> m_activeInputs;
};

#endif