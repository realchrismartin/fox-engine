#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "SFML/Window/Event.hpp"

class Component
{
public:
	virtual void update() = 0;
	virtual void informOfWindowEvent(sf::Event event) {};
	virtual bool caresAboutWindowEvents() const { return false; }
};

#endif