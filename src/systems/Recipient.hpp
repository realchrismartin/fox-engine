#ifndef RECIPIENT_HPP
#define RECIPIENT_HPP

#include "src/systems/MessageTypes.hpp" //Helpful to put this here so recipient subclasses dont need to include it in addition to this.

/// @brief A recipient of messages from the relay
/// @brief Use: subclass this in your class, then implement onMessageReceived<T> to receive messages.
/// @brief If you add a new message type, you need to update Recipient.cpp to add new explicit template instantiation info (Ctor/dtor)
/// @tparam T 
template <typename T>
class Recipient
{
public:
	Recipient<T>();
	~Recipient<T>();

	virtual void onMessageReceived(const T& message){}
};

#endif
