#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/PhysicsComponent.hpp"
#include "src/components/SpriteComponent.hpp"

ExampleTestScene::ExampleTestScene()
{
	createObstacles();
	createPlayer();
	createFloor();
}

void ExampleTestScene::createPlayer()
{
	if (m_playerUID.has_value())
	{
		removeEntity(m_playerUID.value());
	}

	m_playerUID = createEntity();

	if (m_playerUID.has_value()) 
	{
		int uid = m_playerUID.value();

		addComponent<InputComponent>(uid);

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(10.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(50.f, 50.f));
		getComponent<PhysicsComponent>(uid).setHasStaticBody(false);
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(400.f, 300.f));

		addComponent<SpriteComponent>(uid);
	}
}

void ExampleTestScene::createFloor()
{
	if (m_floorUID.has_value())
	{
		removeEntity(m_floorUID.value());
	}

	m_floorUID = createEntity();

	if (m_floorUID.has_value()) 
	{
		int uid = m_floorUID.value();

		addComponent<SpriteComponent>(uid);

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(20.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(1600.f, 50.f));
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(0.f, 1150.f));
	}
}

void ExampleTestScene::createObstacles()
{
	if (m_obstacleUID.has_value())
	{
		removeEntity(m_obstacleUID.value());
	}

	m_obstacleUID = createEntity();

	if (m_obstacleUID.has_value()) 
	{
		int uid = m_obstacleUID.value();

		addComponent<SpriteComponent>(uid);

		addComponent<PhysicsComponent>(uid);
		getComponent<PhysicsComponent>(uid).setMass(5.f);
		getComponent<PhysicsComponent>(uid).setSize(sf::Vector2f(40.f, 40.f));
		getComponent<PhysicsComponent>(uid).setHasStaticBody(false);
		getComponent<PhysicsComponent>(uid).setStartingPosition(sf::Vector2f(450.f, 500.f));
	}
}
