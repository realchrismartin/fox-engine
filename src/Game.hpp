#ifndef GAME_HPP 
#define GAME_HPP

#include "SFML/System/Clock.hpp"
#include "src/Window.hpp"

class Game
{
public:
	Game();
	void play();
private:
	Window m_window;
	sf::Clock m_clock;
};

#endif
