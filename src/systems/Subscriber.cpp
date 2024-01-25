#include "src/systems/Subscriber.hpp"

#include "src/systems/EventRelay.hpp"

//Add new constructors here as you add Event types!

Subscriber<WindowEvent>::Subscriber()
{
	EventRelay::getInstance()->subscribe<WindowEvent>(this);
}

Subscriber<WindowEvent>::~Subscriber()
{
	EventRelay::getInstance()->unsubscribe<WindowEvent>(this);
}

Subscriber<InputEvent>::Subscriber()
{
	EventRelay::getInstance()->subscribe<InputEvent>(this);
}

Subscriber<InputEvent>::~Subscriber()
{
	EventRelay::getInstance()->unsubscribe<InputEvent>(this);
}
