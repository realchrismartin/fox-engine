#include "src/systems/Recipient.hpp"

#include "src/systems/MessageRelay.hpp"

//Add new constructors here as you add message types!

Recipient<SceneChangeMessage>::Recipient()
{
	MessageRelay::getInstance()->subscribe<SceneChangeMessage>(this);
}

Recipient<SceneChangeMessage>::~Recipient()
{
	MessageRelay::getInstance()->unsubscribe<SceneChangeMessage>(this);
}