#include "src/systems/EventRelay.hpp"

const std::shared_ptr<EventRelay> EventRelay::Event_RELAY_SINGLETON = std::shared_ptr<EventRelay>(new EventRelay());

std::shared_ptr<EventRelay> EventRelay::getInstance()
{
    return Event_RELAY_SINGLETON;
}

EventRelay::EventRelay()
{
    //Private, because this is a singleton!
}

