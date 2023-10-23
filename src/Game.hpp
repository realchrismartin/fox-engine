#ifndef GAME_HPP 
#define GAME_HPP

#include "src/Scene.hpp"

class Game
{
public:
	Game();
	void play();
private:
	std::unique_ptr<sf::View> m_camera;
	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<Scene> m_scene;
	std::vector<sf::Event> m_events;
};

#endif
