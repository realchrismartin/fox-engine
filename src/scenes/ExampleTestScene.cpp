#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"

ExampleTestScene::ExampleTestScene()
{
	createPlayer();
	createObstacles();
	createBackground();
	createFloor();
	createBush();
	createHat();
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
		getComponent<TransformComponent>(uid).setScale({ .2f,.2f,.2f });

		ModelData model;
		model.modelFilePath = "../../img/untitled.obj";
		model.spriteSize = { 1024,1024 };
		model.spriteOffsetOnTexture = { 0,511 };
		getComponent<ModelComponent>(uid).loadModel(model);
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
		getComponent<TransformComponent>(uid).setScale({ 50.f, 1.f, 50.f});
		getComponent<TransformComponent>(uid).setTranslation({ 0.f,-2.f,0.f});
		addComponent<ModelComponent>(uid);

		ModelData model;
		model.modelFilePath = "../../img/cube.obj";
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };
		getComponent<ModelComponent>(uid).loadModel(model);
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
		getComponent<TransformComponent>(uid).setTranslation({10.f,0.f,4.f}); 
		addComponent<ModelComponent>(uid);
		setCameraEntity(uid);
	}
}

void ExampleTestScene::createBush()
{
	m_bushUID = createEntity();

	if (m_bushUID.has_value())
	{
		int uid = m_bushUID.value();
		addComponent<TransformComponent>(uid);
		getComponent<TransformComponent>(uid).setTranslation({0.f,0.f,-10.f});
		addComponent<ModelComponent>(uid);
		ModelData model;
		model.modelFilePath = "../../img/quoteunquote-bush.obj";
		model.spriteSize = { 228,228 };
		model.spriteOffsetOnTexture = { 0,513 };
		getComponent<ModelComponent>(uid).loadModel(model);

	}
}

void ExampleTestScene::createHat()
{
	std::optional<int> entityUID = createEntity();

	if (!entityUID.has_value())
	{
		return;
	}

	int uid = entityUID.value();

	addComponent<TransformComponent>(uid);
	getComponent<TransformComponent>(uid).setTranslation({ 10.f,2.5f,0.f });
	addComponent<ModelComponent>(uid);

	ModelData model;
	model.modelFilePath = "../../img/quoteunquote-bush.obj";
	model.spriteSize = { 228,228 };
	model.spriteOffsetOnTexture = { 0,513 };
	getComponent<ModelComponent>(uid).loadModel(model);

	addChild(m_playerUID.value(), uid);
}
