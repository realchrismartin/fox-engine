#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/VerticesComponent.hpp"
#include "src/graphics/ModelConfig.hpp"

ExampleTestScene::ExampleTestScene()
{
	createPlayer();
	createFloor();
	createWindmill();
	createBush();
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
		ModelConfig model;
		model.modelFilePath = "../../img/untitled.obj";
		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setScale({ .2f,.2f,.2f});
		setCameraTargetEntity(uid);
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
		ModelConfig model;
		model.modelFilePath = "../../img/cube.obj";
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };
		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setScale({ 100.f,1.f,100.f });
		getComponent<TransformComponent>(uid).setTranslation({ 0.f,-2.f,0.f});
	}

}

void ExampleTestScene::createWindmill()
{
	if (m_windmillUID.has_value())
	{
		removeEntity(m_windmillUID.value());
	}

	m_windmillUID = createEntity();

	if (m_windmillUID.has_value()) 
	{
		int uid = m_windmillUID.value();
		ModelConfig model;
		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setRotation({ 0.f,180.f,0.f});
	}
}

void ExampleTestScene::createBush()
{
	m_bushUID = createEntity();

	if (m_bushUID.has_value())
	{
		int uid = m_bushUID.value();
		ModelConfig model;
		model.modelFilePath = "../../img/quoteunquote-bush.obj";
		model.spriteSize = { 228,228 };
		model.spriteOffsetOnTexture = { 0,513 };
		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setTranslation({ 10.f,0.f,5.f});
		addChild(m_windmillUID.value(), uid);
	}
}
