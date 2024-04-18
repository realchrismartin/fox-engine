#ifndef TRIGGERCOMPONENT_HPP
#define TRIGGERCOMPONENT_HPP

class Scene;

struct Trigger
{
	/// @brief Sets a condition that will be evaluated once every game update
	/// @param conditionFn 
	void setUpdateCondition(const std::function<bool(Scene&, int, float, float)>& conditionFn)
	{
		m_conditionFn = conditionFn;	
	}

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


private:

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
	void resetLifetime();

private:
	float m_lifetime = 0.f;

	std::vector<Trigger> m_triggers;
};
#endif
