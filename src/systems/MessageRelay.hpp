#ifndef MESSAGERELAY_HPP
#define MESSAGERELAY_HPP

#include <typeindex>
#include <any>

#include "src/systems/Recipient.hpp"

class MessageRelay
{
public:
	static std::shared_ptr<MessageRelay> getInstance();

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

private:
	MessageRelay();
	static const std::shared_ptr<MessageRelay> MESSAGE_RELAY_SINGLETON;

	std::unordered_map<std::type_index, std::vector<std::any>> m_channels;
};
#endif