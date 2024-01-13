#include "src/entities/GameEntityLibrary.hpp"

#include "src/entities/GameEntityEnum.hpp"
#include "src/entities/GameEntityConfig.hpp"

#include "src/components/ComponentPool.hpp"
#include "src/scenes/Scene.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/TriggerComponent.hpp"
#include "src/components/config/ModelConfig.hpp"
#include "src/components/config/TextConfig.hpp"

//Static definitions for different types of entities go here!
namespace GameEntities 
{
	static const GameEntityConfig TITLE_TEXT = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
<<<<<<< HEAD
			TextConfig config;
			config.textToDisplay = "fox n fowl";
			config.charactersPerLine = 10;
			config.centered = true;
<<<<<<< HEAD
			config.margin = { .15f,.15f };
			config.fontSize = 15;
<<<<<<< HEAD
			scene.loadText(config, entity.getUID());
=======
			ModelConfig model;
			model.keyframeFilePaths = { "../../img/cube.obj" };
			scene.loadModel(model, entityUID);

			scene.getComponent<TransformComponent>(entityUID).setScale({ 5.f,5.f,1.f });
>>>>>>> d220627 (oops)
=======
			scene.loadText(config, entityUID);
>>>>>>> 3607be4 (shipit)
=======
			scene.loadText(config, entityUID);
>>>>>>> 593a9c6 (fix)
		});

	static const GameEntityConfig START_BUTTON = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
<<<<<<< HEAD
			TextConfig config;
			config.textToDisplay = "press one to begin";
			config.centered = true;
<<<<<<< HEAD
			config.animated = true;
			config.margin = { .05f,.05f };
			config.fontSize = 5;
<<<<<<< HEAD
			scene.loadText(config, entity.getUID());
=======
			ModelConfig model;
			model.keyframeFilePaths = { "../../img/cube.obj" };
			scene.loadModel(model, entityUID);

			scene.getComponent<TransformComponent>(entityUID).setScale({ 5.f,5.f,1.f });
>>>>>>> d220627 (oops)
=======
			scene.loadText(config, entityUID);
>>>>>>> 3607be4 (shipit)
=======
			scene.loadText(config, entityUID);
>>>>>>> 593a9c6 (fix)
		});

	static const GameEntityConfig PLAYER = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			scene.addComponent<InputComponent>(entityUID);

			ModelConfig model;
			scene.loadModel(model, entityUID);
		});

	static const GameEntityConfig FLOOR = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 512,512 };
			model.spriteOffsetOnTexture = { 511,1988 };
			model.keyframeFilePaths = { "../../img/cube.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 100.f,1.f,100.f });
		});

	static const GameEntityConfig BUSH = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 228,228 };
			model.spriteOffsetOnTexture = { 0,1759 };
			model.keyframeFilePaths = { "../../img/quoteunquote-bush.obj" };

<<<<<<< HEAD
<<<<<<< HEAD
			scene.loadModel(model, entity.getUID());
			scene.getComponent<TransformComponent>(entity.getUID()).setScale({ 2.f,2.f,2.f});
=======
			scene.loadModel(model, entityUID);
>>>>>>> d220627 (oops)
=======
			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 2.f,2.f,2.f});
>>>>>>> 3607be4 (shipit)
		});

	static const GameEntityConfig MUSHROOM = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 1024.f,1024.f };
			model.spriteOffsetOnTexture = { 1023.f,1476 };
			model.frameCount = 60;
			model.keyframeFilePaths = {
				"../../img/mushroom/mushroom0.obj",
				"../../img/mushroom/mushroom5.obj",
				"../../img/mushroom/mushroom10.obj",
				"../../img/mushroom/mushroom15.obj",
				"../../img/mushroom/mushroom20.obj"
			};

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 2.f,2.f,2.f});
		});

	static const GameEntityConfig BLOCK_EMITTER = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{

			//The emitter itself
			ModelConfig model;
			model.keyframeFilePaths = { "../../img/cube.obj" };
			scene.loadModel(model, entityUID);

			//The cubes that get emitted
			for (int i = 0; i < 10; i++)
			{
				std::optional<int> id = scene.createEntity();

				if (!id.has_value())
				{
					continue;
				}
				
				//Add this cube as a child of the emitter
				scene.addChild(entityUID, id.value());

				//Add this cube as an owned entity of the emitter
				//We will use this later when we operate 

				//Add the cube model
				scene.loadModel(model, id.value());

				//TODO: this will start out zeroed instead of this when the below is implemented
				scene.getComponent<TransformComponent>(id.value()).setTranslation({ (float)(rand() % 4),(float)(rand() % 4),(float)(rand() % 4)});

				//Set this entity as inactive
				scene.setEntityActiveStatus(id.value(), false);

				scene.addComponent<TriggerComponent>(id.value());

				TriggerComponent& triggerComponent = scene.getComponent<TriggerComponent>(id.value());

				Trigger trigger;
				trigger.setCondition([](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > 2.f;
				});
				trigger.setAction([](Scene& scene, int entityUID)
				{
					scene.setEntityActiveStatus(entityUID,true);

					TransformComponent& transform = scene.getComponent<TransformComponent>(entityUID);

					const glm::vec3& currentScale = transform.getScale();
					const glm::vec3 newScale = glm::vec3(currentScale.x * 1.1f, currentScale.y * 1.1f, currentScale.z * 1.1f);
					transform.setScale(newScale);
				});

				triggerComponent.addTrigger(trigger);

				//TODO: add the trigger components fns here. 
				//The functions should:

				//When? If the timer is reset
				// a) set the model active at the beginning of the timer

				//When? If the timer is running but not complete
				// b) apply translation to the model each tick to simulate movement

				//When? If the timer is complete
				// c) .reset() the TransformComponent and set active=false when the timer expires. moving the cube back to the emitter source
			}
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
			return GameEntities::MUSHROOM;
		case(GameEntityEnum::BLOCK_EMITTER):
			return GameEntities::BLOCK_EMITTER;
		default:
			return GameEntities::BUSH; //quote unqoute
	}
}
