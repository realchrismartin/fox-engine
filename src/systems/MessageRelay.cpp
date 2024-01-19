#include "src/systems/MessageRelay.hpp"

const std::shared_ptr<MessageRelay> MessageRelay::MESSAGE_RELAY_SINGLETON = std::shared_ptr<MessageRelay>(new MessageRelay());

std::shared_ptr<MessageRelay> MessageRelay::getInstance()
{
    return MESSAGE_RELAY_SINGLETON;
}

MessageRelay::MessageRelay()
{
    //Private, because this is a singleton!
}

