#ifndef GAMEENTITYLIBRARY_HPP
#define GAMEENTITYLIBRARY_HPP

#include "src/components/ComponentPool.hpp"
#include "src/scenes/Scene.hpp"
#include "src/entities/GameEntityConfig.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/graphics/ModelConfig.hpp"

/// @brief All of the game entities that can be added to a scene.
enum class GameEntityEnum
{
	TITLE_TEXT,
	START_BUTTON,
	PLAYER,
	FLOOR,
	BUSH,
	MUSHROOM
};

//Static definitions for different types of entites go here!

static GameEntityConfig TITLE_TEXT = GameEntityConfig()
	.whenInit([](auto& entity, auto& scene)
	{
		ModelConfig model;
		model.keyframeFilePaths = { "../../img/cube.obj" };
		scene.loadModel(model, entity.getUID());

		scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 5.f,5.f,1.f });
	});


static GameEntityConfig START_BUTTON = GameEntityConfig()
	.whenInit([](auto& entity, auto& scene)
	{
		ModelConfig model;
		model.keyframeFilePaths = { "../../img/cube.obj" };
		scene.loadModel(model, entity.getUID());

		scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 5.f,5.f,1.f });
	});


static GameEntityConfig PLAYER = GameEntityConfig()
	.whenInit([](auto& entity, auto& scene)
	{
		scene.addComponent<InputComponent>(entity.getUID());

		ModelConfig model;
		scene.loadModel(model, entity.getUID());
	});

static GameEntityConfig FLOOR = GameEntityConfig()
	.whenInit([](auto& entity, auto& scene)
	{
		ModelConfig model;
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };
		model.keyframeFilePaths = { "../../img/cube.obj" };

		scene.loadModel(model, entity.getUID());
		scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 100.f,1.f,100.f });
	});

static GameEntityConfig BUSH = GameEntityConfig()
	.whenInit([](auto& entity, auto& scene)
	{
		ModelConfig model;
		model.spriteSize = { 228,228 };
		model.spriteOffsetOnTexture = { 0,513 };
		model.keyframeFilePaths = { "../../img/quoteunquote-bush.obj" };

		scene.loadModel(model, entity.getUID());
	});

static GameEntityConfig MUSHROOM = GameEntityConfig()
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

namespace GameEntityLibrary
{
	/// @brief Get a reference to the static game entity config for a given entity type
	/// @param gameEntity 
	/// @return 
	inline GameEntityConfig& getGameEntityConfig(GameEntityEnum gameEntity)
	{
		switch (gameEntity)
		{
		case(GameEntityEnum::TITLE_TEXT):
			return TITLE_TEXT;
		case(GameEntityEnum::START_BUTTON):
			return START_BUTTON;
		case(GameEntityEnum::PLAYER):
			return PLAYER;
		case(GameEntityEnum::FLOOR):
			return FLOOR;
		case(GameEntityEnum::BUSH):
			return BUSH;
		case(GameEntityEnum::MUSHROOM):
			return MUSHROOM;
		default:
			return MUSHROOM;
		}
	}
}

#endif