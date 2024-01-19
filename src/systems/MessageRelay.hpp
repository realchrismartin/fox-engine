#ifndef MESSAGERELAY_HPP
#define MESSAGERELAY_HPP

#include <typeindex>
#include <any>

#include "src/systems/Recipient.hpp"

/// @brief A singleton class that is responsible for holding "channels" and allowing anything to send messages to these channels.
/// @brief By subclassing Recipient<T>, a class will be subscribed to a given channel and its "onMessageReceived<T>" method will be called when messages are sent to the channel
class MessageRelay
{
public:
	/// @brief Static accessor. Call this to get ahold of the message relay to send messages!
	/// @return 
	static std::shared_ptr<MessageRelay> getInstance();

	/// @brief Send a message to a specific channel based on the type of message being sent.
	/// @tparam T 
	/// @param message 
	template <typename T>
	void sendMessage(const T& message)
	{
		auto type = std::type_index(typeid(T));

		if (!m_channels.count(type))
		{
			return;
		}

		for (auto const& sub : m_channels.at(type))
		{
			std::any_cast<Recipient<T>*>(sub)->onMessageReceived(message);
		}
	}



	/// @brief Subscribe to a channel as a message recipient
	/// @tparam T 
	/// @param sub 
	template <typename T>
	void subscribe(Recipient<T>* sub)
	{
		auto type = std::type_index(typeid(T));

		if (!m_channels.count(type))
		{
			std::vector<std::any> anyVec;
			m_channels.emplace(type, anyVec);
		}

		m_channels.at(type).push_back(sub);
	}

	/// @brief Unsubscribe from a channel as a message recipient
	/// @tparam T 
	/// @param sub 
	template <typename T>
	void unsubscribe(Recipient<T>* sub)
	{
		auto type = std::type_index(typeid(T));

		if (!m_channels.count(type))
		{
			return;
		}

		std::erase_if(m_channels.at(type), [sub](auto item)
		{
			return std::any_cast<Recipient<T>*>(item) == sub;
		});
	}

private:
	MessageRelay();
	static const std::shared_ptr<MessageRelay> MESSAGE_RELAY_SINGLETON;

	std::unordered_map<std::type_index, std::vector<std::any>> m_channels;
};
#endif