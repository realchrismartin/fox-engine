#ifndef GAME_HPP 
#define GAME_HPP

#include "SFML/Graphics/RenderWindow.hpp"

class Game
{
public:
	Game();
	void play();
	~Game();
private:
	std::unique_ptr<sf::RenderWindow> m_window;
};

#endif
