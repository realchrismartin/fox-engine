#include "Game.hpp"
#include <optional>

#include "src/util/Logger.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/components/GraphicsComponent.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"

#include "src/systems/WindowEventSystem.hpp"
#include "src/systems/InputProcessingSystem.hpp"
#include "src/systems/GraphicsTransformSystem.hpp"
#include "src/systems/RenderSystem.hpp"

Game::Game()
{
	//This is all of the setup for the game.

	//TODO: later, move the window and its view outta here!
	//Make a window for our game.
	m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Chris and J's Game");
	m_window->setFramerateLimit(60);

	//Make a "camera" that's a view of the window.
	m_camera = std::make_unique<sf::View>(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	//m_camera->rotate(50.f);
	m_window->setView(*m_camera);

	//Set up the scene (a holder for entities and components associated with those entities) and add all of its systems
	//TODO: later, perhaps we should consider decoupling systems from the scene, so we can re-use systems.
	//TODO: alternately, systems could be entirely static, fuckit
	m_scene = std::make_unique<Scene>();
	m_scene->addSystem(std::move(std::make_shared<WindowEventSystem>()));
	m_scene->addSystem(std::move(std::make_shared<InputProcessingSystem>()));
	m_scene->addSystem(std::move(std::make_shared<GraphicsTransformSystem>()));
	m_scene->addSystem(std::move(std::make_shared<RenderSystem>()));

	//Set up some test entities in the scene, for now
	//TODO: later, move these entities and their compnents to some concept of a "level" or something - or make an easier way to bootstrap a scene!

	//Create a "player" that responds to inputs
	std::optional<int> playerEntityUID = m_scene->createEntity();
	if (playerEntityUID.has_value()) 
	{
		m_scene->addComponent<InputComponent>(playerEntityUID.value());
		m_scene->addComponent<TransformComponent>(playerEntityUID.value());
		m_scene->addComponent<GraphicsComponent>(playerEntityUID.value());
	}

	//Create an "obstacle" that, uh, DOESN'T respond to inputs.
	std::optional<int> obstacleEntityUID = m_scene->createEntity();
	if (obstacleEntityUID.has_value()) 
	{
		m_scene->addComponent<GraphicsComponent>(obstacleEntityUID.value());
		m_scene->getComponent<GraphicsComponent>(obstacleEntityUID.value()).setColor(sf::Color::Red);
		m_scene->addComponent<TransformComponent>(obstacleEntityUID.value());
		m_scene->getComponent<TransformComponent>(obstacleEntityUID.value()).setPosition(sf::Vector2f(200.f,230.f));
	}

	//Say hi!
	Logger::log("Welcome to Chris n' J's game, you freak!");
}

void Game::play()
{
 	//This is the main game loop.
	 while (m_window->isOpen())
	 {

		 //Grab hold of all sf::Events that occurred since the last tick.
		 m_events.clear();
		 sf::Event event;
		 while (m_window->pollEvent(event))
		 {
			 if (event.type == sf::Event::Closed)
			 {
				m_window->close();
			 }

			 m_events.push_back(event);
		 }

		 //TODO: we need some semblance of a constant time tick per update here.

		 //Clear the window.
		 m_window->clear();

		 //Update the scene. This will both do updates AND draw to the screen for now
		 //We pass in the window (to maybe be updated) and the events so that they can be handed off to the scene's systems.
		 //TODO: later, we may consider not having the scene know about windows or events at all. Leave that to the systems!
		 m_scene->update(m_window, m_events);

		 //Display whatever has been drawn to the window since the clear happened, i.e. all of the stuff that was drawn during update.
		 m_window->display();
	}
}
