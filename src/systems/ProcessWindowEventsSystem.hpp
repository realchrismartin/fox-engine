#ifndef PROCESSWINDOWEVENTSSYSTEM_HPP 
#define PROCESSWINDOWEVENTSSYSTEM_HPP 

#include "src/systems/System.hpp"

class ProcessWindowEventsSystem: public System
{
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) override;
};

#endif
