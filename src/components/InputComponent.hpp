#ifndef INPUTCOMPONENT_HPP
#define INPUTCOMPONENT_HPP

#include "src/systems/Subscriber.hpp"
#include "src/systems/EventTypes.hpp"

enum class UserInputActionsEnum
{
	PRESSING_W,
	PRESSING_S,
	PRESSING_A,
	PRESSING_D,
	LEFT_CLICKING,
	RIGHT_CLICKING,
	NONE
};

/// @brief A component which stores events coming from sf::RenderWindow and filters out the ones we don't care about. In the end, it tracks the events that are actively occurring that are "input" related
class InputComponent : public Subscriber<InputEvent>
{
public:
	void onEvent(const InputEvent& event) override;

	//TODO: remove these later
	glm::vec3 getTranslationGivenInput();
	glm::vec3 getRotationGivenInput();

	/// @brief Return true if any input is active
	/// @return 
	bool anyInputActive() const;
	
	/// @brief Get all currently active inputs
	/// @return 
	const std::set<UserInputActionsEnum>& getActiveInputs() const;

	/// @brief Get the coordinates a mouse action was activated at within the window, if it's active currently, otherwise (or if it's a keyboard action) return std::nullopt.
	/// @param action 
	/// @return 
	std::optional<glm::vec2> getInputActionWindowCoordinates(UserInputActionsEnum action) const;
private:

	static UserInputActionsEnum getInputActionForScancode(SDL_Scancode code);
	void markKeyInputState(UserInputActionsEnum action, bool pressed);
	void markMouseInputState(UserInputActionsEnum action, bool pressed, const glm::vec2& coordinates);

	std::set<UserInputActionsEnum> m_activeInputs;
	std::unordered_map<UserInputActionsEnum, glm::vec2> m_actionCoordinates;
	float m_translationFactor = .25f; //TODO
};

#endif