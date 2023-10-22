#ifndef GAME_HPP 
#define GAME_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

#include "src/GameEntityManager.hpp"
#include "src/ComponentManager.hpp"
#include "src/GraphicsComponent.hpp"
#include "src/TransformComponent.hpp"
#include "src/InputComponent.hpp"

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

	ComponentManager<InputComponent> m_inputComponentManager;
	ComponentManager<GraphicsComponent> m_graphicsComponentManager;
	ComponentManager<TransformComponent> m_transformComponentManager;
	GameEntityManager m_gameEntityManager;
};

#endif
