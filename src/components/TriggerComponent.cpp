#include "TriggerComponent.hpp"
<<<<<<< HEAD
#include "TriggerComponent.hpp"
=======
>>>>>>> a31d035 (rebrand and crashy)

void TriggerComponent::update(Scene& scene, int entityUID, float elapsedTime)
{
	m_lifetime += elapsedTime;

	for (auto const& trigger : m_triggers)
	{
		if (trigger.shouldRun(scene, entityUID, m_lifetime, elapsedTime))
		{
			trigger.run(scene, entityUID);
		}
	}
}

void TriggerComponent::addTrigger(const Trigger& trigger)
{
	m_triggers.push_back(trigger);
}

void TriggerComponent::resetLifetime()
{
	m_lifetime = 0.f;
}
