#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include <vector>
#include "src/Component.hpp"

enum class UserInputActionsEnum
{
	PRESSED_W,
	PRESSED_S,
	PRESSED_A,
	PRESSED_D
};

class InputComponent : public Component
{
public:
	InputComponent();
	void update() override;
	void informOfWindowEvent(sf::Event event) override;
	bool caresAboutWindowEvents() const;
	std::vector<UserInputActionsEnum>& getActionsSinceLastUpdate();
private:
	std::vector<UserInputActionsEnum> m_actionSinceLastUpdate;
};

#endif