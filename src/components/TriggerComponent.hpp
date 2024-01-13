#ifndef TRIGGERCOMPONENT_HPP
#define TRIGGERCOMPONENT_HPP

class TriggerComponent
{
public:
	void update(float elapsedTime);

	//Somehow contain a method or function
	//We want to be able to trigger the method/function within the scene when an event happens

private:
	float m_lifetime = 0.f;
};
#endif
