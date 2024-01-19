#ifndef RECIPIENT_HPP
#define RECIPIENT_HPP

#include "src/systems/MessageTypes.hpp" //Helpful to put this here so recipient subclasses dont need to include it in addition to this.

template <typename T>
class Recipient
{
public:
	Recipient<T>();
	~Recipient<T>();

	virtual void onMessageReceived(const T& message){}
};

#endif
