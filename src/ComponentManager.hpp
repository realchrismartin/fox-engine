#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#include <unordered_map>
#include <cassert>
#include "SFML/Graphics/RenderWindow.hpp"

template <typename T>
class ComponentManager
{
public:
	ComponentManager() {};

	void addComponent(int uid)
	{
		if (hasComponentForEntityWithID(uid))
		{
			return;
		}

		m_components.emplace(uid, T());
	}

	void updateComponents()
	{
		for (auto& [uid,component] : m_components)
		{
			component.update();
		}
	}

	void informComponentsOfWindowEvent(sf::Event event)
	{
		if (m_components.empty() || !m_components[0].caresAboutWindowEvents())
		{
			return;
		}

		for (auto& [uid,component] : m_components)
		{
			component.informOfWindowEvent(event);
		}
	}

	bool hasComponentForEntityWithID(int uid) const
	{
		return m_components.count(uid);
	}

	T& getComponentForEntityWithID(int uid)
	{
		if (!hasComponentForEntityWithID(uid))
		{
			assert(false); //That doesnt exist chief
		}

		return m_components.at(uid);
	}

private:
	std::unordered_map<int,T> m_components;
};

#endif
