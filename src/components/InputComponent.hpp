#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include "SFML/Window/Event.hpp"

enum class UserInputActionsEnum
{
	PRESSING_W,
	PRESSING_S,
	PRESSING_A,
	PRESSING_D
};

/// @brief A component which stores events coming from sf::RenderWindow and filters out the ones we don't care about. In the end, it tracks the events that are actively occurring that are "input" related
class InputComponent
{
public:
	void informOfWindowEvent(sf::Event event);
	const std::set<UserInputActionsEnum>& getActiveInputs() const;
	const std::set<int>& getChildren() const { return m_children; }
	void addChild(int child) {}
	void removeChild(int child) {}
private:
	std::set<UserInputActionsEnum> m_activeInputs;
	std::set<int> m_children;
};

#endif