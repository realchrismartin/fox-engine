#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/graphics/ModelConfig.hpp"
#include <format>

ExampleTestScene::ExampleTestScene()
{
	createPlayer();
	createFloor();
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
		model.spriteSize = { 1024.f,1024.f };
		model.spriteOffsetOnTexture = { 1023.f,0 };
		model.frameCount = 60;
		model.keyframeFilePaths = {
			"../../img/mushroom/mushroom0.obj",
			"../../img/mushroom/mushroom5.obj",
			"../../img/mushroom/mushroom10.obj",
			"../../img/mushroom/mushroom15.obj",
			"../../img/mushroom/mushroom20.obj"
		};
		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setScale({ 2.f,2.f,2.f});
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
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };
		model.keyframeFilePaths = { "../../img/cube.obj" };

		loadModel(model, uid);

		getComponent<TransformComponent>(uid).setScale({ 100.f,1.f,100.f });
		getComponent<TransformComponent>(uid).setTranslation({ 0.f,-2.f,0.f});
	}

}

void ExampleTestScene::createBush()
{
	m_bushUID = createEntity();

	if (m_bushUID.has_value())
	{
		int uid = m_bushUID.value();

		ModelConfig model;
		model.spriteSize = { 228,228 };
		model.spriteOffsetOnTexture = { 0,513 };
		model.keyframeFilePaths = { "../../img/quoteunquote-bush.obj" };

		loadModel(model, uid);

		getComponent<TransformComponent>(uid).setTranslation({ 10.f,0.f,5.f});
	}
}
