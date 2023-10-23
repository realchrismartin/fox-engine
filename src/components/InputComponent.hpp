#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

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
	const std::vector<UserInputActionsEnum>& getActionsSinceLastUpdate() const;
private:
	std::vector<UserInputActionsEnum> m_actionSinceLastUpdate;
};

#endif