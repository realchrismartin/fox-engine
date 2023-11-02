#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"

ExampleTestScene::ExampleTestScene()
{
	//NB: right now there is no z indexing. Create your stuff in order!
	createBackground();
	createFloor();
	createObstacles();
	createPlayer();
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
		getComponent<TransformComponent>(uid).setScale({ .5f,.5f,.5f });
		getComponent<TransformComponent>(uid).setRotation({ 0.f,0.f,0.f });
		addComponent<ModelComponent>(uid);
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
		getComponent<TransformComponent>(uid).setScale({ 5.f,.2f,1.f});
		getComponent<TransformComponent>(uid).setPosition({ -1.f,-1.f,1.f});
		addComponent<ModelComponent>(uid);
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
