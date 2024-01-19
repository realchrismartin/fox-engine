#include "src/systems/Recipient.hpp"

#include "src/systems/MessageRelay.hpp"

//Add new constructors here as you add message types!

Recipient<WindowMessage>::Recipient()
{
	MessageRelay::getInstance()->subscribe<WindowMessage>(this);
}

Recipient<WindowMessage>::~Recipient()
{
	MessageRelay::getInstance()->unsubscribe<WindowMessage>(this);
}

Recipient<DefaultMessage>::Recipient()
{
	MessageRelay::getInstance()->subscribe<DefaultMessage>(this);
}

Recipient<DefaultMessage>::~Recipient()
{
	MessageRelay::getInstance()->unsubscribe<DefaultMessage>(this);
}