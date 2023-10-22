#include "Game.hpp"
#include <optional>

#include "src/Logger.hpp"

#include "src/GameEntity.hpp"
#include "src/components/GraphicsComponent.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"

Game::Game()
{
	m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Chris and J's Game");
	m_window->setFramerateLimit(60);

	m_camera = std::make_unique<sf::View>(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	//m_camera->rotate(50.f);
	m_window->setView(*m_camera);

	m_scene = std::make_unique<Scene>();

	initTestEntities();

	Logger::log("Welcome to Chris n' J's game, you freak!");
}

 void Game::initTestEntities()
{
	std::optional<int> playerEntityUID = m_scene->createEntity();
	if (playerEntityUID.has_value()) {
		m_scene->addComponent<InputComponent>(playerEntityUID.value());
		m_scene->addComponent<TransformComponent>(playerEntityUID.value());
		m_scene->addComponent<GraphicsComponent>(playerEntityUID.value());
	}
	

	std::optional<int> obstacleEntityUID = m_scene->createEntity();
	if (obstacleEntityUID.has_value()) {
		m_scene->addComponent<GraphicsComponent>(obstacleEntityUID.value());
		m_scene->getComponent<GraphicsComponent>(obstacleEntityUID.value())->setColor(sf::Color::Red);
		m_scene->addComponent<TransformComponent>(obstacleEntityUID.value());
		m_scene->getComponent<TransformComponent>(obstacleEntityUID.value())->setPosition(sf::Vector2f(100.f, 100.f));
	}
}

void Game::play()
{
	 while (m_window->isOpen())
	 {
		 sf::Event event;
		 while (m_window->pollEvent(event))
		 {
			 if (event.type == sf::Event::Closed)
			 {
				m_window->close();
			 }

			 m_events.push_back(event);
		 }

		 m_window->clear();
		 m_scene->update(m_window, m_events);
		 m_events.clear();
		 m_window->display();
	}
}
