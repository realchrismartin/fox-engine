#include "src/scenes/SceneLibrary.hpp"

#include "src/scenes/SceneConfig.hpp"
#include "src/scenes/SceneEnum.hpp"
#include "src/scenes/Scene.hpp"

#include "src/entities/GameEntityEnum.hpp"
#include "src/entities/GameEntityLibrary.hpp"
#include "src/entities/EntityInstanceConfig.hpp"

#include "src/components/TransformComponent.hpp"

#include "src/util/Logger.hpp"

namespace Scenes
{
	static const SceneConfig MAIN_MENU = SceneLibrary::initSceneConfig(SceneEnum::MAIN_MENU);
	static const SceneConfig LEVEL_1 = SceneLibrary::initSceneConfig(SceneEnum::LEVEL_1);
}

/// @brief Get a statically allocated scene configuration. This will be called from Scene:: when a new scene is requested.
/// @brief It will always return a reference to the single statically allocated config for the scene enum specified.
/// @param scene 
/// @return 
const SceneConfig& SceneLibrary::getSceneConfig(SceneEnum scene)
{
	switch (scene)
	{
		case(SceneEnum::LEVEL_1):
			return Scenes::LEVEL_1;
		case(SceneEnum::MAIN_MENU):
		default:
			return Scenes::MAIN_MENU;
	}
}

/// @brief Build a scene config for the scene. This should not be called directly except when statically initializing scenes.
/// @brief Define your scene's contents here!
/// @param scene 
/// @return 
const SceneConfig SceneLibrary::initSceneConfig(SceneEnum scene)
{
	SceneConfig config;
	config.id = scene;

	switch(scene)
	{
	case(SceneEnum::LEVEL_1):
	{
		//Player
		auto player = config.addEntity(GameEntityEnum::PLAYER);
		player.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,5.f,0.f });
		});

		//Floor
		auto floor = config.addEntity(GameEntityEnum::FLOOR);
		floor.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-2.f,0.f });
		});

		//Bush
		auto bush = config.addEntity(GameEntityEnum::BUSH);
		bush.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 10.f,0.f,5.f });
		});

<<<<<<< HEAD
=======
		//Add the bush as a child of the player
		player.addChild(bush);

>>>>>>> 014ca54 (j's pr review)
		//Mushroom
		auto mushroom1 = config.addEntity(GameEntityEnum::MUSHROOM);
		mushroom1.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 10.f,0.f,5.f });
		});

		//Another mushroom
		auto mushroom2 = config.addEntity(GameEntityEnum::MUSHROOM);
		mushroom2.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,0.f,10.f });
<<<<<<< HEAD
=======

			//Example: when a message is received, get twice as big!
			scene.addComponent<TriggerComponent>(entityUID);
			TriggerComponent& tc = scene.getComponent<TriggerComponent>(entityUID);

			//Example direct trigger sink
			Trigger directTrigger;
			directTrigger.setDirectTriggerCondition(true);
			directTrigger.setAction([](Scene& scene, int entityUID)
			{
				if (!scene.hasComponent<TransformComponent>(entityUID))
				{
					return;
				}
				
				scene.getComponent<TransformComponent>(entityUID).setRotation({ 90.f,0.f,2.f });
			});

			//Example message trigger sink
			Trigger messageTrigger;
			messageTrigger.setMessageCondition(true);
			messageTrigger.setAction([](Scene& scene, int entityUID)
			{
				if (!scene.hasComponent<TransformComponent>(entityUID))
				{
					return;
				}
				
				scene.getComponent<TransformComponent>(entityUID).setScale({ 5.f,3.f,2.f });
			});

			tc.addTrigger(directTrigger);
			tc.addTrigger(messageTrigger);
		});

		//Mushroom
		auto mushroom1 = config.addEntity(GameEntityEnum::MUSHROOM);
		mushroom1.addInitFn([mushroom2](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 10.f,0.f,5.f });

			//Send a message when it's been alive for 5s to the other mushroom
			scene.addComponent<TriggerComponent>(entityUID);
			TriggerComponent& tc = scene.getComponent<TriggerComponent>(entityUID);

			//Example direct trigger source
			Trigger directTrigger;
			directTrigger.setUpdateCondition([](Scene& scene, int entityUID, float lifetime, float elapsedTime)
			{
				return lifetime > 6.f;
			});

			directTrigger.setAction([mushroom2](Scene& scene, int entityUID)
			{
				scene.trigger(mushroom2.entityUID);
			});

			//Example message trigger source
			Trigger messageTrigger;
			messageTrigger.setUpdateCondition([](Scene& scene, int entityUID, float lifetime, float elapsedTime)
			{
				return lifetime > 3.f;
			});

			messageTrigger.setAction([mushroom2](Scene& scene, int entityUID)
			{
				TriggerExecutionMessage message;
				message.entityUID = mushroom2.entityUID;
				MessageRelay::getInstance()->sendMessage(message);
			});


			tc.addTrigger(messageTrigger);
			tc.addTrigger(directTrigger);
>>>>>>> 586c5df (variouse)
		});

<<<<<<< HEAD
=======
		auto emitter = config.addEntity(GameEntityEnum::BLOCK_EMITTER);
<<<<<<< HEAD
>>>>>>> 3607be4 (shipit)
=======
		emitter.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,2.f,0.f });
		});

		config.addChild(player, emitter);
>>>>>>> 5a80cce (magnificent)

		break;
	}

	case(SceneEnum::MAIN_MENU):
	{
		//Title
		auto title = config.addEntity(GameEntityEnum::TITLE_TEXT);
		title.addInitFn([](int entityUID, auto& scene)
		{
<<<<<<< HEAD
<<<<<<< HEAD
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({0.f,.5f,0.f });
=======
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,20.f,15.f });
>>>>>>> 014ca54 (j's pr review)
=======
			scene.getComponent<TransformComponent>(entityUID).setTranslation({0.f,.5f,0.f });
>>>>>>> 3607be4 (shipit)
		});

		//Start button
		auto startButton = config.addEntity(GameEntityEnum::START_BUTTON);
		startButton.addInitFn([](int entityUID, auto& scene)
		{
<<<<<<< HEAD
<<<<<<< HEAD
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({0.f,-.3f,0.f });
=======
			scene.getComponent<TransformComponent>(entityUID).setTranslation({0.f,-.3f,0.f });
>>>>>>> 3607be4 (shipit)
		});

		//Floor
		auto floor = config.addEntity(GameEntityEnum::FLOOR);
		floor.addInitFn([](int entityUID, auto& scene)
		{
<<<<<<< HEAD
<<<<<<< HEAD
			scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 1000.f,1000.f,1.f });
			scene.getComponent<TransformComponent>(entity.getUID()).setTranslation({ 0.f,-2.f,-2.f });
			scene.getComponent<TransformComponent>(entity.getUID()).setRotation({ 45.f,0.f,0.f });
=======
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,5.f,15.f });
			scene.setCameraTargetEntity(entityUID);
>>>>>>> 014ca54 (j's pr review)
=======
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-2.f,-2.f });
			scene.getComponent<TransformComponent>(entityUID).setRotation({ 45.f,0.f,0.f });
>>>>>>> 3607be4 (shipit)
=======
			scene.getComponent<TransformComponent>(entityUID).setScale({ 1000.f,1000.f,1.f });
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-2.f,-2.f });
			scene.getComponent<TransformComponent>(entityUID).setRotation({ 45.f,0.f,0.f });
>>>>>>> 593a9c6 (fix)
		});

		break;
	}
	case(SceneEnum::NONE):
	default:
		break;
	}

	return config;
}

