#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"

ExampleTestScene::ExampleTestScene()
{
	//TODO: depth testing is not working
	createObstacles();
	createFloor();
	createPlayer();
	createBackground();
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

		addComponent<TransformComponent>(uid);
		addComponent<ModelComponent>(uid);
		getComponent<ModelComponent>(uid).setTextureCoordinates(sf::Vector2i(0, 0));
		getComponent<ModelComponent>(uid).setSpriteSize(sf::Vector2i(49,49));

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
		addComponent<TransformComponent>(uid);
		getComponent<TransformComponent>(uid).setScale({ 100.f,.2f,100.f});
		getComponent<TransformComponent>(uid).setPosition({ -1.f,-1.f,1.f});
		addComponent<ModelComponent>(uid);
		getComponent<ModelComponent>(uid).setTextureCoordinates(sf::Vector2i(0, 50));
		getComponent<ModelComponent>(uid).setSpriteSize(sf::Vector2i(11, 100));
	}
}

void ExampleTestScene::createBackground()
{
	if (m_backgroundUID.has_value())
	{
		removeEntity(m_backgroundUID.value());
	}

	m_backgroundUID = createEntity();

	if (m_backgroundUID.has_value()) 
	{
		int uid = m_backgroundUID.value();

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
		addComponent<TransformComponent>(uid);
	}
}
