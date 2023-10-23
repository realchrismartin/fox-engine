#ifndef INPUTPROCESSINGSYSTEM_HPP 
#define INPUTPROCESSINGSYSTEM_HPP

#include "src/systems/System.hpp"

class InputProcessingSystem : public System
{
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) override;
};

#endif