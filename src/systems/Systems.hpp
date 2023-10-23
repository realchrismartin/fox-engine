#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/Scene.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/PhysicsComponent.hpp"
#include "src/components/GraphicsComponent.hpp"

class Systems
{
public:
	//Run all of the game systems that pertain to updating
	static const void update(Scene& scene, sf::RenderWindow& window, float elapsedTime)
	{
		runWindowEventSystem(scene, window);
		runInputProcessingSystem(scene, elapsedTime);
		runPhysicsSystem(scene, elapsedTime);
		runRenderSystem(scene, window);
	};

	//Run all of the game systems that pertain to rendering
	//Assumes we called update first.
	static const void render(Scene& scene, sf::RenderWindow& window)
	{
		//Clear the window.
		window.clear();

		//Draw stuf to the screen
		runRenderSystem(scene, window);

		//Display whatever has been drawn to the window since the clear happened, i.e. all of the stuff that was drawn during update.
		window.display();
	};
private:
	static const void runWindowEventSystem(Scene& scene, sf::RenderWindow& window)
	{
		std::vector<sf::Event> events;

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close(); //TODO: mayb move this later? IDK!
			}

			events.push_back(event);
		}

		for (auto const& entity : EntityFilter<InputComponent>(scene))
		{
			InputComponent& input = scene.getComponent<InputComponent>(entity.getUID());

			input.clearEvents();

			for (auto const& event : events)
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

		for (auto const& entity : EntityFilter<PhysicsComponent>(scene))
		{
			PhysicsComponent& physicsComponent = scene.getComponent<PhysicsComponent>(entity);

			//Update each physics component to pass time, update itself based on time passed
			physicsComponent.update(elapsedTime);

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
		}

		//Update each graphics component associated with a physics component to relocate it, if needed.
		for (auto const& entity : EntityFilter<PhysicsComponent, GraphicsComponent>(scene))
		{
			scene.getComponent<GraphicsComponent>(entity).updateWithPhysics(scene.getComponent<PhysicsComponent>(entity));
		}
	}

	static const void runRenderSystem(Scene& scene, sf::RenderWindow& window)
	{
		for (auto const& entity : EntityFilter<GraphicsComponent>(scene))
		{
			scene.getComponent<GraphicsComponent>(entity.getUID()).draw(window);
		}
	}
};

#endif