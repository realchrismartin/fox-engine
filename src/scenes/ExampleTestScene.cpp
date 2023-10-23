#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/components/GraphicsComponent.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/PhysicsComponent.hpp"

ExampleTestScene::ExampleTestScene()
{
	//Set up some test entities in the scene, for now
	//TODO: later, move these entities and their compnents to some concept of a "level" or something - or make an easier way to bootstrap a scene!

	//Create a "player" that responds to inputs
	std::optional<int> playerEntity = createEntity();

	if (playerEntity.has_value()) 
	{
		int uid = playerEntity.value();
		addComponent<InputComponent>(uid);

		addComponent<GraphicsComponent>(uid);
		getComponent<GraphicsComponent>(uid).setColor(sf::Color::Blue);
		getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(50.f, 50.f));

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(10.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(50.f, 50.f));
		getComponent<PhysicsComponent>(uid).setHasStaticBody(false);
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(400.f, 300.f));
	}

	//Create a dynamic "obstacle"
	std::optional<int> obstacleEntity = createEntity();

	if (obstacleEntity.has_value()) 
	{
		int uid = obstacleEntity.value();

		addComponent<GraphicsComponent>(uid);
		getComponent<GraphicsComponent>(uid).setColor(sf::Color::Green);
		getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(40.f, 40.f));

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(5.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(40.f, 40.f));
		getComponent<PhysicsComponent>(uid).setHasStaticBody(false);
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(450.f, 500.f));
	}

	//Create a larger dynamic "obstacle"
	std::optional<int> obstacle2Entity = createEntity();

	if (obstacle2Entity.has_value()) 
	{
		int uid = obstacle2Entity.value();

		addComponent<GraphicsComponent>(uid);
		getComponent<GraphicsComponent>(uid).setColor(sf::Color::Cyan);
		getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(70.f, 75.f));

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(90.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(70.f, 75.f));
		getComponent<PhysicsComponent>(uid).setHasStaticBody(false);
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(550.f, 400.f));
	}

	std::optional<int> floorEntity = createEntity();

	if (floorEntity.has_value()) 
	{
		int uid = floorEntity.value();
		addComponent<GraphicsComponent>(uid);
		getComponent<GraphicsComponent>(uid).setColor(sf::Color::Red);
		getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(20.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 550.f));
	}

	std::optional<int> ceilingEntity = createEntity();

	if (ceilingEntity.has_value()) 
	{
		int uid = ceilingEntity.value();
		addComponent<GraphicsComponent>(uid);
		getComponent<GraphicsComponent>(uid).setColor(sf::Color::Red);
		getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(20.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(800.f, 50.f));
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 0.f));
	}

	std::optional<int> leftWallEntity = createEntity();

	if (leftWallEntity.has_value()) 
	{
		int uid = leftWallEntity.value();
		addComponent<GraphicsComponent>(uid);
		getComponent<GraphicsComponent>(uid).setColor(sf::Color::Red);
		getComponent<GraphicsComponent>(uid).setSize(sf::Vector2f(50.f, 600.f));

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(20.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(50.f, 600.f));
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 0.f));
	}
}
