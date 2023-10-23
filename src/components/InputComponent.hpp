#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include "SFML/Window/Event.hpp"

enum class UserInputActionsEnum
{
	PRESSING_A,
	PRESSING_D,
	PRESSING_SPACEBAR
};

class InputComponent
{
public:
	void informOfWindowEvent(sf::Event event);
	const std::set<UserInputActionsEnum>& getActiveInputs() const;
private:
	std::set<UserInputActionsEnum> m_activeInputs;
};

#endif