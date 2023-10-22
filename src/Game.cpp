#include "Game.hpp"

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

	int playerEntityUID = m_scene->createEntity();
	m_scene->addComponent<InputComponent>(playerEntityUID);
	m_scene->addComponent<TransformComponent>(playerEntityUID);
	m_scene->addComponent<GraphicsComponent>(playerEntityUID);

	int obstacleEntityUID = m_scene->createEntity();
	m_scene->addComponent<GraphicsComponent>(obstacleEntityUID);
	m_scene->getComponent<GraphicsComponent>(obstacleEntityUID)->setColor(sf::Color::Red);
	m_scene->addComponent<TransformComponent>(obstacleEntityUID);
	m_scene->getComponent<TransformComponent>(obstacleEntityUID)->setPosition(sf::Vector2f(100.f, 100.f));
}

void Game::play()
{
	 while (m_window->isOpen())
	 {
		 //TODO: move the systems
		 for (auto const& entity : m_scene->getEntities())
		 {
			InputComponent* input = m_scene->getComponent<InputComponent>(entity.getUID());

			if (input != nullptr)
			{
				input->clearEvents();
			}
		 }

		 sf::Event event;

		 while (m_window->pollEvent(event))
		 {
			 if (event.type == sf::Event::Closed)
			 {
				m_window->close();
			 }

			 //TODO: move the systems
			 for (auto const& entity : m_scene->getEntities())
			 {
				InputComponent* input = m_scene->getComponent<InputComponent>(entity.getUID());

				if (input != nullptr)
				{
					input->informOfWindowEvent(event);
				}
			 }
		 }

		 m_window->clear();


		 //TODO: move the systems
		 for (auto const& entity : m_scene->getEntities())
		 {
		 	 InputComponent* input = m_scene->getComponent<InputComponent>(entity.getUID());
			 TransformComponent* transform = m_scene->getComponent<TransformComponent>(entity.getUID());
			 GraphicsComponent* graphics = m_scene->getComponent<GraphicsComponent>(entity.getUID());
			 
			 if (input != nullptr && transform != nullptr)
			 {
				 transform->updateWithInput(input);
			 }

			 if (transform != nullptr && graphics != nullptr)
			 {
				 graphics->updateWithTransform(transform);
			 }

			 if (graphics != nullptr)
			 {
				 graphics->draw(m_window);
			 }
		 }
		 m_window->display();
	}
}

Game::~Game()
{
}