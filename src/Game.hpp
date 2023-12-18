#ifndef GAME_HPP 
#define GAME_HPP

#include "src/Clock.hpp"
#include "src/graphics/Window.hpp"

/// @brief The core game class. Main.cpp instantiates this and calls play(), play will run until the game says to stop!
class Game
{
public:
	void play();
private:
	bool initSDL();
	static const float TIMESTEP;
};

#endif
