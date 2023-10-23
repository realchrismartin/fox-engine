#ifndef WINDOWEVENTSYSTEMSYSTEM_HPP 
#define WINDOWEVENTSYSTEMSYSTEM_HPP 

#include "src/systems/System.hpp"

class WindowEventSystem: public System
{
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) override;
};

#endif
