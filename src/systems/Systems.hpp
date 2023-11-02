#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/Window.hpp"
#include "src/scenes/Scene.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"

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

		//Move transforms around with the input
		for (auto const& entity : EntityFilter<InputComponent, TransformComponent>(scene))
		{
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
			InputComponent& input = scene.getComponent<InputComponent>(entity);

			glm::vec3 position = transform.getPosition();
			glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);

			for (auto const& enumValue : input.getActiveInputs())
			{
				switch (enumValue)
				{
					//TODO
					case(UserInputActionsEnum::PRESSING_W):
					{
						position += glm::vec3(0.f,0.f, .1f);
						rotation += glm::vec3(0.f, 180.f, 0.f);
						break;
					}
					case(UserInputActionsEnum::PRESSING_S):
					{
						position += glm::vec3(0.f,0.f, -.1f);
						break;
					}
					case(UserInputActionsEnum::PRESSING_A):
					{
						position += glm::vec3(.1f, 0.f, 0.f);
						rotation += glm::vec3(0.f, 90.f, 0.f);
						break;
					}
					case(UserInputActionsEnum::PRESSING_D):
					{
						position += glm::vec3(-.1f, 0.f, 0.f);
						rotation += glm::vec3(0.f, -90.f, 0.f);
						break;
					}
					default:
					{
						break;
					}
				}
			}

			transform.setRotation(rotation);
			transform.setPosition(position);
		}
	}

	static const void runRenderSystem(Scene& scene, Window& window)
	{

		//View matrix
		glm::mat4 viewMatrix = glm::lookAt(
			glm::vec3(0.f,10.f,-10.f), 
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f)
		);

		// Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)1600.f / (float)1200.f, 0.1f, 100.0f);

		for (auto const& entity : EntityFilter<TransformComponent, ModelComponent>(scene))
		{
			ModelComponent& model = scene.getComponent<ModelComponent>(entity);
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
			glm::mat4 modelMatrix = transform.getModelMatrix();

			glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

			//Bind the model matrix via uniform
			window.getBoundShader().updateMat4Uniform("modelViewProjectionMatrix", mvp);

			//Draw the bits of the model
			window.draw(model.getVertices().size(), model.getIndices().size(), model.getVertices(), model.getIndices());
		}
	}
};

#endif