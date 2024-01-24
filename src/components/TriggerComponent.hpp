#ifndef TRIGGERCOMPONENT_HPP
#define TRIGGERCOMPONENT_HPP

<<<<<<< HEAD
=======
#include <queue>;
#include "src/systems/MessageTypes.hpp"
#include "src/systems/Recipient.hpp"
#include "src/components/InputComponent.hpp"

>>>>>>> 3ef75ed (upgrade events and allow for window resizing)
class Scene;


struct Trigger
{
<<<<<<< HEAD
	void setCondition(const std::function<bool(Scene&, int, float, float)>& conditionFn)
=======
	/// @brief Sets a condition that will be evaluated once every game update
	/// @param conditionFn 
	void setUpdateCondition(const std::function<bool(Scene&, int, float, float)>& conditionFn)
>>>>>>> 586c5df (variouse)
	{
		m_conditionFn = conditionFn;	
	}

<<<<<<< HEAD
=======
	/// @brief Indicates that if a message is received, this trigger should fire
	/// @param useMessageCondition 
	void setMessageCondition(bool useMessageCondition)
	{
		m_supportsMessageExecution = useMessageCondition;
	}

	/// @brief Indicates that if the scene is asked to trigger directly, this trigger should fire
	/// @param useDirectTrigger 
	void setDirectTriggerCondition(bool useDirectTrigger)
	{
		m_supportsDirectExecution = useDirectTrigger;
	}

>>>>>>> 586c5df (variouse)
	void setAction(const std::function<void(Scene&, int)>& actionFn)
	{
		m_actionFn = actionFn;
	}

	bool shouldRun(Scene& scene, int entityUID, float lifetime, float elapsedTime) const
	{
		return m_conditionFn(scene,entityUID,lifetime,elapsedTime);
	}

	void run(Scene& scene, int entityUID) const
	{
		m_actionFn(scene, entityUID);
	}

<<<<<<< HEAD
private:
=======
	bool supportsMessageExecution() const
	{
		return m_supportsMessageExecution;
	}

	bool supportsDirectExecution() const
	{
		return m_supportsDirectExecution;
	}

private:

	bool m_supportsMessageExecution = false;

	bool m_supportsDirectExecution = false;

>>>>>>> 586c5df (variouse)
	std::function<bool(Scene&,int, float,float)> m_conditionFn = [](Scene& scene, int entityUID, float lifetime, float elapsedTime) 
	{
		return false;
	};

	std::function<void(Scene&, int)> m_actionFn = [](Scene& scene, int entityUID)
	{
	};
};

class TriggerComponent
{
public:
	void update(Scene& scene, int entityUID, float elapsedTime);
	void addTrigger(const Trigger& trigger);
	void triggerDirectly(Scene& scene, int entityUID);
	void resetLifetime();

private:
	float m_lifetime = 0.f;

	std::vector<Trigger> m_triggers;
};
#endif
