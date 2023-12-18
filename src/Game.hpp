#ifndef GAME_HPP 
#define GAME_HPP

#include "SFML/System/Clock.hpp"
#include "src/Window.hpp"

/// @brief The core game class. Main.cpp instantiates this and calls play(), play will run until the game says to stop!
class Game
{
public:
	Game();
	void play();
private:
	bool initSDL();
	Window m_window;
	sf::Clock m_clock;
	static const float TIMESTEP;
};

#endif
