#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

class Scene;

class System
{
public:
	virtual void update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) = 0;
};

#endif