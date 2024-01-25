#ifndef SUBSCRIBER_HPP 
#define SUBSCRIBER_HPP 

#include "src/systems/EventTypes.hpp" //Helpful to put this here so Subscriber subclasses dont need to include it in addition to this.

/// @brief A Subscriber of Events from the relay
/// @brief Use: subclass this in your class, then implement onEvent<T> to receive Events.
/// @brief If you add a new Event type, you need to update Subscriber.cpp to add new explicit template instantiation info (Ctor/dtor)
/// @tparam T 
template <typename T>
class Subscriber
{
public:
	Subscriber<T>();
	~Subscriber<T>();

	virtual void onEvent(const T& event){}
};

#endif
