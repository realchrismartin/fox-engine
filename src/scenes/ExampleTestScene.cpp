#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/PhysicsComponent.hpp"
#include "src/components/RectangleShapeComponent.hpp"
#include "src/components/SpriteComponent.hpp"

ExampleTestScene::ExampleTestScene()
{
	//Set up some test entities in the scene, for now
	//TODO: later, move these entities and their compnents to some concept of a "level" or something - or make an easier way to bootstrap a scene!

	//Create a background

	std::optional<int> backgroundEntity = createEntity();

	if (backgroundEntity.has_value()) 
	{
		int uid = backgroundEntity.value();

		addComponent<SpriteComponent>(uid);
		getComponent<SpriteComponent>(uid).setTexturePath("../../img/background.png");
		getComponent<SpriteComponent>(uid).setSize(sf::Vector2f(1600.f, 1200.f));
		getComponent<SpriteComponent>(uid).setPosition(sf::Vector2f(0.f, 0.f));
	}

	//Create a "player" that responds to inputs
	std::optional<int> playerEntity = createEntity();

	if (playerEntity.has_value()) 
	{
		int uid = playerEntity.value();
		addComponent<InputComponent>(uid);

		addComponent<SpriteComponent>(uid);
		getComponent<SpriteComponent>(uid).setTexturePath("../../img/character.png");

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

		addComponent<RectangleShapeComponent>(uid);
		getComponent<RectangleShapeComponent>(uid).setFillColor(sf::Color::Red);

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

		addComponent<RectangleShapeComponent>(uid);
		getComponent<RectangleShapeComponent>(uid).setFillColor(sf::Color::Cyan);

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
		addComponent<RectangleShapeComponent>(uid);
		getComponent<RectangleShapeComponent>(uid).setFillColor(sf::Color::Green);

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(20.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(1600.f, 50.f));
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 1150.f));
	}

	std::optional<int> leftWallEntity = createEntity();

	if (leftWallEntity.has_value()) 
	{
		int uid = leftWallEntity.value();
		addComponent<RectangleShapeComponent>(uid);
		getComponent<RectangleShapeComponent>(uid).setFillColor(sf::Color::Red);

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(20.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(50.f, 100.f));
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 1100.f));
	}
}
