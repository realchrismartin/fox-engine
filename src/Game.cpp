#include "Game.hpp"

#include "src/GameEntity.hpp"

#include <iostream>

Game::Game()
{
	m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Chris and J's Game");
	m_window->setFramerateLimit(60);

	m_camera = std::make_unique<sf::View>(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	//m_camera->rotate(50.f);
	m_window->setView(*m_camera);

	initTestEntities();
}

void Game::initTestEntities()
{
	//The Player!
	int entity1ID = m_gameEntityManager.createEntity();
	m_inputComponentManager.addComponent(entity1ID);
	m_transformComponentManager.addComponent(entity1ID);
	m_graphicsComponentManager.addComponent(entity1ID);

	//Some other stuff.
	int entity2ID = m_gameEntityManager.createEntity();
	m_graphicsComponentManager.addComponent(entity2ID);
	m_graphicsComponentManager.getComponentForEntityWithID(entity2ID).setColor(sf::Color::Red);
	m_transformComponentManager.addComponent(entity2ID);
	m_transformComponentManager.getComponentForEntityWithID(entity2ID).setPosition(sf::Vector2f(0.f, 150.f));

	int entity3ID = m_gameEntityManager.createEntity();
	m_graphicsComponentManager.addComponent(entity3ID);
	m_graphicsComponentManager.getComponentForEntityWithID(entity3ID).setColor(sf::Color::Red);
	m_transformComponentManager.addComponent(entity3ID);
	m_transformComponentManager.getComponentForEntityWithID(entity3ID).setPosition(sf::Vector2f(150.f,0.f));
}

void Game::play()
{
	 while (m_window->isOpen())
	 {
		 sf::Event event;

		 m_inputComponentManager.updateComponents();
		 m_transformComponentManager.updateComponents();
		 m_graphicsComponentManager.updateComponents();

		 while (m_window->pollEvent(event))
		 {
			 // Request for closing the window
			 if (event.type == sf::Event::Closed)
			 {
				m_window->close();
			 }

			 m_inputComponentManager.informComponentsOfWindowEvent(event);
		 }

		 m_window->clear();

		 //For each of our game enti
		 for (auto const& entity : m_gameEntityManager.getGameEntities())
		 {
			 int uid = entity.getUID();

			 if (m_inputComponentManager.hasComponentForEntityWithID(uid) && m_transformComponentManager.hasComponentForEntityWithID(uid))
			 {
				 InputComponent& inputComponent = m_inputComponentManager.getComponentForEntityWithID(entity.getUID());
				 m_transformComponentManager.getComponentForEntityWithID(entity.getUID()).updateWithInput(inputComponent);
			 }

			 //System 2: transform > graphics
			 if (m_transformComponentManager.hasComponentForEntityWithID(uid) && m_graphicsComponentManager.hasComponentForEntityWithID(uid))
			 {
				 TransformComponent& transformComponent = m_transformComponentManager.getComponentForEntityWithID(entity.getUID());
				 m_graphicsComponentManager.getComponentForEntityWithID(entity.getUID()).updateWithTransform(transformComponent);
			 }

			 //System 3: draw graphics!
			 if (m_graphicsComponentManager.hasComponentForEntityWithID(uid))
			 {
				 m_graphicsComponentManager.getComponentForEntityWithID(entity.getUID()).draw(m_window);
			 }
		 }

		 m_window->display();
	}
}

Game::~Game()
{
}