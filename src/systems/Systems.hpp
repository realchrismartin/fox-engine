#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/Window.hpp"
#include "src/scenes/Scene.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/PhysicsComponent.hpp"
#include "src/components/RectangleShapeComponent.hpp"
#include "src/components/SpriteComponent.hpp"

/// @brief A collection of static functions that are "systems", functions that operate on specific associations of components in a scene to update them.
/// @brief The update and render meta-systems are the core of the game. 
class Systems
{
public:
	//Run all of the game systems that pertain to updating
	static const void update(Scene& scene, Window& window, float elapsedTime)
	{
		runWindowEventSystem(scene, window);
		runInputProcessingSystem(scene, elapsedTime);
		runPhysicsSystem(scene, elapsedTime);
	};

	//Run all of the game systems that pertain to rendering
	//Assumes we called update first.
	static const void render(Scene& scene, Window& window)
	{
		//Clear the window.
		window.clear();

		//Draw stuff to the window
		runRenderSystem(scene, window);

		//Display whatever has been drawn to the window since the clear happened, i.e. all of the stuff that was drawn during update.
		window.display();
	};
private:
	static const void runWindowEventSystem(Scene& scene, Window& window)
	{
		window.pollForEvents();

		for (auto const& entity : EntityFilter<InputComponent>(scene))
		{
			InputComponent& input = scene.getComponent<InputComponent>(entity.getUID());

			for (auto const& event : window.getEvents())
			{
				input.informOfWindowEvent(event);
			}
		}
	}

	static const void runInputProcessingSystem(Scene& scene, float elapsedTime)
	{
		for (auto const& entity : EntityFilter<InputComponent, PhysicsComponent>(scene))
		{
			scene.getComponent<PhysicsComponent>(entity).updateWithInput(scene.getComponent<InputComponent>(entity), elapsedTime);
		}
	}

	static const void runPhysicsSystem(Scene& scene, float elapsedTime)
	{
		//Update each physics component which is associated with input to update based on input
		for (auto const& entity : EntityFilter<PhysicsComponent, InputComponent>(scene))
		{
			scene.getComponent<PhysicsComponent>(entity).updateWithInput(scene.getComponent<InputComponent>(entity), elapsedTime);
		}

		//Update collisions for all entities
		for (auto const& entity : EntityFilter<PhysicsComponent>(scene))
		{
			PhysicsComponent& physicsComponent = scene.getComponent<PhysicsComponent>(entity);

			for (auto const& otherEntity : EntityFilter<PhysicsComponent>(scene))
			{
				if (entity.getUID() != otherEntity.getUID())
				{
					//Assume these are not the same components either, check for collision.
					PhysicsComponent& otherPhysicsComponent = scene.getComponent<PhysicsComponent>(otherEntity);
					
					physicsComponent.updateCollision(otherPhysicsComponent, elapsedTime);
					otherPhysicsComponent.updateCollision(physicsComponent, elapsedTime);
				}
			}

			//Update each physics component to pass time, update itself based on time passed
		    //NB: has to run after collision updates.
			physicsComponent.update(elapsedTime);
		}

		//Update each graphics component associated with a physics component to relocate/resize it
		for (auto const& entity : EntityFilter<PhysicsComponent, SpriteComponent>(scene))
		{
			scene.getComponent<SpriteComponent>(entity).updateWithPhysics(scene.getComponent<PhysicsComponent>(entity));
		}

		for (auto const& entity : EntityFilter<PhysicsComponent, RectangleShapeComponent>(scene))
		{
			scene.getComponent<RectangleShapeComponent>(entity).updateWithPhysics(scene.getComponent<PhysicsComponent>(entity));
		}
	}

	static const void runRenderSystem(Scene& scene, Window& window)
	{
		//lol what is z indexing
		//TODO

		//Draw all the sprites 
		for (auto const& entity : EntityFilter<SpriteComponent>(scene))
		{
			window.draw(scene.getComponent<SpriteComponent>(entity.getUID()));
		}

		//Draw all the rectangles
		for (auto const& entity : EntityFilter<RectangleShapeComponent>(scene))
		{
			window.draw(scene.getComponent<RectangleShapeComponent>(entity.getUID()));
		}
	}
};

#endif