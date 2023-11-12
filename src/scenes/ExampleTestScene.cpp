#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/VerticesComponent.hpp"
#include "src/graphics/ModelData.hpp"

ExampleTestScene::ExampleTestScene()
{
	createFloor();
	createPlayer();
	createObstacles();
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
		ModelData model;
		model.modelFilePath = "../../img/untitled.obj";
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };
		loadModel(model, uid);
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
		ModelData model;
		model.modelFilePath = "../../img/cube.obj";
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };
		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setScale({ 100.f,1.f,100.f });
		getComponent<TransformComponent>(uid).setTranslation({ 0.f,-1.f,0.f});
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
		ModelData model;
		model.modelFilePath = "../../img/windmill.obj";
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };

	}
}

void ExampleTestScene::createBush()
{
	m_bushUID = createEntity();

	if (m_bushUID.has_value())
	{
		int uid = m_bushUID.value();
		ModelData model;
		model.modelFilePath = "../../img/quoteunquote-bush.obj";
		model.spriteSize = { 228,228 };
		model.spriteOffsetOnTexture = { 0,513 };
		loadModel(model, uid);
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

	ModelData model;
	model.modelFilePath = "../../img/quoteunquote-bush.obj";
	model.spriteSize = { 228,228 };
	model.spriteOffsetOnTexture = { 0,513 };
	loadModel(model, uid);

	addChild(m_playerUID.value(), uid);
}
