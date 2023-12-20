#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/config/ModelConfig.hpp"
#include "src/components/config/TextConfig.hpp"
#include <format>

ExampleTestScene::ExampleTestScene()
{
	createPlayer();
	createFloor();
	createBush();
	createText();
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
		model.spriteOffsetOnTexture = { 1017,1476 };
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
		model.spriteOffsetOnTexture = { 511,1989 }; //TODO slightly misaligned and inside mushroom //1989
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
		model.spriteOffsetOnTexture = { 0,1761 };
		model.keyframeFilePaths = { "../../img/quoteunquote-bush.obj" };

		loadModel(model, uid);

		getComponent<TransformComponent>(uid).setTranslation({ 0.f,0.f,0.f});
	}
}

void ExampleTestScene::createText()
{
	m_textUID = createEntity();

	if (m_textUID.has_value())
	{
		int uid = m_textUID.value();
		TextConfig config;
		config.textToDisplay = "j is sleepy and this is quite a long bit of text isnt it? why yes i do beleive that it is. it is quite longe. in fact we could write a whole essay in here and im sure it would FIT ON THE SCREEN am i right";
		loadText(config, uid);
		getComponent<TransformComponent>(uid).setTranslation({ -1.f,0.f,0.f});
	}

	m_text2UID = createEntity();

	if (m_text2UID.has_value())
	{
		int uid = m_text2UID.value();
		TextConfig config;
		config.fontSize = 4;
		config.textToDisplay = "and lo and behoold heere is an otehr text! but it is slightly smaller is it not";
		config.charactersPerLine = 10;
		loadText(config, uid);
		getComponent<TransformComponent>(uid).setTranslation({ .5f,.5f,0.f});
	}

}
