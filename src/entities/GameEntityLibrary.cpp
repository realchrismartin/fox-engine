#include "src/entities/GameEntityLibrary.hpp"

#include "src/entities/GameEntityEnum.hpp"
#include "src/entities/GameEntityConfig.hpp"

#include "src/components/ComponentPool.hpp"
#include "src/scenes/Scene.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/graphics/ModelConfig.hpp"

//Static definitions for different types of entities go here!
namespace GameEntities 
{
	static const GameEntityConfig TITLE_TEXT = GameEntityConfig()
		.whenInit([](auto& entity, auto& scene)
		{
			ModelConfig model;
			model.keyframeFilePaths = { "../../img/cube.obj" };
			scene.loadModel(model, entity.getUID());

			scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 5.f,5.f,1.f });
		});


	static const GameEntityConfig START_BUTTON = GameEntityConfig()
		.whenInit([](auto& entity, auto& scene)
		{
			ModelConfig model;
			model.keyframeFilePaths = { "../../img/cube.obj" };
			scene.loadModel(model, entity.getUID());

			scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 5.f,5.f,1.f });
		});


	static const GameEntityConfig PLAYER = GameEntityConfig()
		.whenInit([](auto& entity, auto& scene)
		{
			scene.addComponent<InputComponent>(entity.getUID());

			ModelConfig model;
			scene.loadModel(model, entity.getUID());
		});

	static const GameEntityConfig FLOOR = GameEntityConfig()
		.whenInit([](auto& entity, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 512,512 };
			model.spriteOffsetOnTexture = { 511,0 };
			model.keyframeFilePaths = { "../../img/cube.obj" };

			scene.loadModel(model, entity.getUID());
			scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 100.f,1.f,100.f });
		});

	static const GameEntityConfig BUSH = GameEntityConfig()
		.whenInit([](auto& entity, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 228,228 };
			model.spriteOffsetOnTexture = { 0,513 };
			model.keyframeFilePaths = { "../../img/quoteunquote-bush.obj" };

			scene.loadModel(model, entity.getUID());
		});

	static const GameEntityConfig MUSHROOM = GameEntityConfig()
		.whenInit([](auto& entity, auto& scene)
		{
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

			scene.loadModel(model, entity.getUID());
			scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 2.f,2.f,2.f});
		});
}

const GameEntityConfig& GameEntityLibrary::getGameEntityConfig(GameEntityEnum gameEntity)
{
	switch (gameEntity)
	{
		case(GameEntityEnum::TITLE_TEXT):
			return GameEntities::TITLE_TEXT;
		case(GameEntityEnum::START_BUTTON):
			return GameEntities::START_BUTTON;
		case(GameEntityEnum::PLAYER):
			return GameEntities::PLAYER;
		case(GameEntityEnum::FLOOR):
			return GameEntities::FLOOR;
		case(GameEntityEnum::BUSH):
			return GameEntities::BUSH;
		case(GameEntityEnum::MUSHROOM):
		default:
			return GameEntities::MUSHROOM;
	}
}
