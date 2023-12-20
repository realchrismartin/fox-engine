#ifndef SCENELIBRARY_HPP
#define SCENELIBRARY_HPP

#include "src/scenes/SceneEnum.hpp"
#include "src/entities/GameEntityLibrary.hpp"
#include "src/util/Logger.hpp"
#include "src/scenes/SceneConfig.hpp"

//Main menu scene
static const SceneConfig MAIN_MENU = SceneConfig()
	.addEntity(GameEntityEnum::TITLE_TEXT,
		[](const auto& entity, auto& scene) 
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,20.f,15.f });
		})
	.addEntity(GameEntityEnum::START_BUTTON,
		[](const auto& entity, auto& scene) 
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,5.f,15.f });
			scene.setCameraTargetEntity(entity.getUID());
		})
	.addEntity(GameEntityEnum::MUSHROOM,
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 5.f,-5.f,10.f });
		})
	.addEntity(GameEntityEnum::MUSHROOM,
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,-5.f,10.f });
		})
	.addEntity(GameEntityEnum::MUSHROOM,
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ -5.f,-5.f,10.f });
		})
	.addEntity(GameEntityEnum::MUSHROOM,
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 5.f,2.f,10.f });
		})
	.addEntity(GameEntityEnum::MUSHROOM,
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,-10.f,10.f });
		})
	.addEntity(GameEntityEnum::MUSHROOM,
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ -5.f,2.f,10.f });
		});


//Test scene
static const SceneConfig TEST_SCENE = SceneConfig()
	.addEntity(GameEntityEnum::FLOOR, //0
		[](const auto& entity, auto& scene) 
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,-2.f,0.f });
		})
	.addEntity(GameEntityEnum::PLAYER) //1
	.addEntity(GameEntityEnum::BUSH, //2
		[](const auto& entity, auto& scene) 
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 10.f,0.f,5.f });
		})
	.addChild(1, 2) //Bush is attached to the player!
	.addEntity(GameEntityEnum::MUSHROOM, //3
		[](const auto& entity, auto& scene) 
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 10.f,0.f,5.f });
		})
	.addEntity(GameEntityEnum::MUSHROOM, //4
		[](const auto& entity, auto& scene)
		{
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,0.f,10.f });
		});

namespace SceneLibrary
{
	inline const SceneConfig& getSceneConfig(SceneEnum scene)
	{
		switch (scene)
		{
			case(SceneEnum::TEST_SCENE):
				return TEST_SCENE;
			case(SceneEnum::MAIN_MENU):
			default:
				return MAIN_MENU;
		}
	}
}

#endif
