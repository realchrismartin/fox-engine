#ifndef GAME_HPP 
#define GAME_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

#include "src/Scene.hpp"

class Game
{
public:
	Game();
	void play();
	~Game();
protected:
	void initTestEntities();
private:
	std::unique_ptr<sf::View> m_camera;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<Scene> m_scene;
};

#endif
