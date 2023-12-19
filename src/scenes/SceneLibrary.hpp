#ifndef SCENELIBRARY_HPP
#define SCENELIBRARY_HPP

#include "src/entities/GameEntityLibrary.hpp"
#include "src/util/Logger.hpp"
#include "src/scenes/SceneConfig.hpp"

/// @brief All of the scenes available to us.
enum class SceneEnum
{
	TEST_SCENE
};

/// @brief A test scene. NB: Any scene-specific configuration goes here. All entity-specific config goes in the GameEntityLibrary!
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
			default:
				return TEST_SCENE;
		}
	}
}

#endif
