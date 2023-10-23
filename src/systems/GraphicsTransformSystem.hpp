#ifndef GRAPHICSTRANSFORMSYSTEM_HPP 
#define GRAPHICSTRANSFORMSYSTEM_HPP

#include "src/systems/System.hpp"

class GraphicsTransformSystem : public System
{
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) override;
};

#endif