#ifndef GIANTMAINSYSTEM_HPP
#define GIANTMAINSYSTEM_HPP

#include "src/systems/System.hpp"

class GiantMainSystem : public System
{
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) override;
};

#endif