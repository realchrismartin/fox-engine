#ifndef EVENTRELAY_HPP
#define EVENTRELAY_HPP

#include <typeindex>
#include <any>

#include "src/systems/Subscriber.hpp"

/// @brief A singleton class that is responsible for holding "channels" and allowing anything to send Events to these channels.
/// @brief By subclassing Subscriber<T>, a class will be subscribed to a given channel and its "onEvent<T>" method will be called when Events are sent to the channel
class EventRelay
{
public:
	/// @brief Static accessor. Call this to get ahold of the Event relay to send Events!
	/// @return 
	static std::shared_ptr<EventRelay> getInstance();

	/// @brief Send a Event to a specific channel based on the type of Event being sent.
	/// @tparam T 
	/// @param Event 
	template <typename T>
	void sendEvent(const T& event)
	{
		auto type = std::type_index(typeid(T));

		if (!m_channels.count(type))
		{
			return;
		}

		for (auto const& sub : m_channels.at(type))
		{
			std::any_cast<Subscriber<T>*>(sub)->onEvent(event);
		}
	}



	/// @brief Subscribe to a channel as a Event Subscriber
	/// @tparam T 
	/// @param sub 
	template <typename T>
	void subscribe(Subscriber<T>* sub)
	{
		auto type = std::type_index(typeid(T));

		if (!m_channels.count(type))
		{
			std::vector<std::any> anyVec;
			m_channels.emplace(type, anyVec);
		}

		m_channels.at(type).push_back(sub);
	}

	/// @brief Unsubscribe from a channel as a Event Subscriber
	/// @tparam T 
	/// @param sub 
	template <typename T>
	void unsubscribe(Subscriber<T>* sub)
	{
		auto type = std::type_index(typeid(T));

		if (!m_channels.count(type))
		{
			return;
		}

		std::erase_if(m_channels.at(type), [sub](auto item)
		{
			return std::any_cast<Subscriber<T>*>(item) == sub;
		});
	}

private:
	EventRelay();
	static const std::shared_ptr<EventRelay> Event_RELAY_SINGLETON;

	std::unordered_map<std::type_index, std::vector<std::any>> m_channels;
};
#endif