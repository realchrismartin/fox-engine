#include "Game.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/GraphicsComponent.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/PhysicsComponent.hpp"

#include "src/systems/Systems.hpp"

Game::Game()
{
	//This is all of the setup for the game.

	//TODO: later, move the window and its view outta here!
	//Make a window for our game.
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Chris and J's Game");
	m_window->setFramerateLimit(60);

	m_clock = std::make_unique<sf::Clock>();

	//Make a "camera" that's a view of the window.
	m_camera = std::make_unique<sf::View>(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	//m_camera->rotate(50.f);
	m_window->setView(*m_camera);

	//Set up the scene (a holder for entities and components associated with those entities)
	m_scene = std::make_unique<Scene>();

	//Set up some test entities in the scene, for now
	//TODO: later, move these entities and their compnents to some concept of a "level" or something - or make an easier way to bootstrap a scene!

	//Create a "player" that responds to inputs
	std::optional<int> playerEntity = m_scene->createEntity();

	if (playerEntity.has_value()) 
	{
		int uid = playerEntity.value();
		m_scene->addComponent<InputComponent>(uid);
		m_scene->addComponent<PhysicsComponent>(uid);

		m_scene->addComponent<GraphicsComponent>(uid);
		m_scene->getComponent<GraphicsComponent>(uid).setColor(sf::Color::Blue);
		m_scene->getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(50.f, 50.f));

		m_scene->getComponent<PhysicsComponent>(uid).setMass(10.f);
		m_scene->getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(50.f, 50.f));
		m_scene->getComponent<PhysicsComponent>(uid).setIsStaticBody(false);
		m_scene->getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(400.f, 300.f));
	}

	//Create an floor that, uh, DOESN'T respond to inputs and doesn't have physics (currently). It just sits there.
	std::optional<int> floorEntity = m_scene->createEntity();

	if (floorEntity.has_value()) 
	{
		int uid = floorEntity.value();
		m_scene->addComponent<GraphicsComponent>(uid);
		m_scene->getComponent<GraphicsComponent>(uid).setColor(sf::Color::Red);
		m_scene->getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));

		m_scene->addComponent<PhysicsComponent>(uid);
		m_scene->getComponent<PhysicsComponent>(uid).setMass(20.f);
		m_scene->getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));
		m_scene->getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 550.f));
	}

	std::optional<int> ceilingEntity = m_scene->createEntity();

	if (ceilingEntity.has_value()) 
	{
		int uid = ceilingEntity.value();
		m_scene->addComponent<GraphicsComponent>(uid);
		m_scene->getComponent<GraphicsComponent>(uid).setColor(sf::Color::Red);
		m_scene->getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));

		m_scene->addComponent<PhysicsComponent>(uid);
		m_scene->getComponent<PhysicsComponent>(uid).setMass(20.f);
		m_scene->getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));
		m_scene->getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 0.f));
	}

	//Say hi!
	Logger::log("Welcome to Chris n' J's game, you freak!");
}

void Game::play()
{
 	 //This is the main game loop.
     //TODO: we need some semblance of a constant time tick per update here.
	 while (m_window->isOpen())
	 {
		 Systems::update(*m_scene, *m_window, m_clock->restart().asMilliseconds());
		 Systems::render(*m_scene, *m_window);
	}
}
