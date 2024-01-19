#ifndef RECIPIENT_HPP
#define RECIPIENT_HPP

template <typename T>
class Recipient
{
public:
	virtual void onMessageReceived(const T& message)
	{
		//Does nothing by default.
	}
};

#endif