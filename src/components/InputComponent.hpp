#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

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
	glm::vec3 getTranslationGivenInput();
	glm::vec3 getRotationGivenInput();
	bool anyInputActive() const;
private:
	std::set<UserInputActionsEnum> m_activeInputs;
	float m_translationFactor = .25f; //TODO
};

#endif