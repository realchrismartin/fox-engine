#ifndef RENDERSYSTEM_HPP 
#define RENDERSYSTEM_HPP

#include "src/systems/System.hpp"

class RenderSystem : public System
{
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) override;
};

#endif