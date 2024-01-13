#include "TriggerComponent.hpp"

void TriggerComponent::update(float elapsedTime)
{
	m_lifetime += elapsedTime;
}
