#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include <vector>
#include "SFML/Window/Event.hpp"

enum class UserInputActionsEnum
{
	PRESSED_W,
	PRESSED_S,
	PRESSED_A,
	PRESSED_D
};

class InputComponent
{
public:
	void clearEvents();
	void informOfWindowEvent(sf::Event event);
	std::vector<UserInputActionsEnum>& getActionsSinceLastUpdate();
private:
	std::vector<UserInputActionsEnum> m_actionSinceLastUpdate;
};

#endif