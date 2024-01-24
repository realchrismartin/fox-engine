#include "src/systems/Recipient.hpp"

#include "src/systems/MessageRelay.hpp"

//Add new constructors here as you add message types!

Recipient<WindowMessage>::Recipient()
{
	MessageRelay::getInstance()->subscribe<WindowMessage>(this);
}

Recipient<WindowMessage>::~Recipient()
{
<<<<<<< HEAD
	MessageRelay::getInstance()->unsubscribe<SceneChangeMessage>(this);
=======
	MessageRelay::getInstance()->unsubscribe<WindowMessage>(this);
}

Recipient<InputMessage>::Recipient()
{
	MessageRelay::getInstance()->subscribe<InputMessage>(this);
}

Recipient<InputMessage>::~Recipient()
{
	MessageRelay::getInstance()->unsubscribe<InputMessage>(this);
}

Recipient<TriggerExecutionMessage>::Recipient()
{
	MessageRelay::getInstance()->subscribe<TriggerExecutionMessage>(this);
}

Recipient<TriggerExecutionMessage>::~Recipient()
{
	MessageRelay::getInstance()->unsubscribe<TriggerExecutionMessage>(this);
>>>>>>> 3ef75ed (upgrade events and allow for window resizing)
}