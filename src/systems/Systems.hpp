#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/Window.hpp"
#include "src/scenes/Scene.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"
#include <functional>

/// @brief A collection of static functions that are "systems", functions that operate on specific associations of components in a scene to update them.
/// @brief The update and render meta-systems are the core of the game. 
class Systems
{
public:
	//Run all of the game systems that pertain to updating
	static const void update(Scene& scene, Window& window, float elapsedTime)
	{
		runWindowEventSystem(scene, window);
		runSceneGraphUpdateSystem(scene);
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

	static const void runSceneGraphUpdateSystem(Scene& scene)
	{

		std::function<glm::mat4& (int, Scene&, glm::mat4&, TransformComponent&)> accumulator = [](int entityUID, Scene& scene, glm::mat4& matrix, TransformComponent& component) -> glm::mat4&
		{
			//TODO: we can do more with the dirty flag here.
			component.updateWorldMatrix(matrix);
			return component.getWorldMatrix();
		};

		glm::mat4 modelMatrix = glm::mat4(1.0);
		scene.applyAccumulatorToSceneGraph<TransformComponent, glm::mat4>(modelMatrix, accumulator);
	}

	static const void runInputProcessingSystem(Scene& scene, float elapsedTime)
	{
		//Move transforms around with the input
		for (auto const& entity : EntityFilter<InputComponent, TransformComponent>(scene))
		{
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
			InputComponent& input = scene.getComponent<InputComponent>(entity);

			//TODO: move me!

			/*
			glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
			glm::vec3 rotation = glm::vec3(0.f, 90.f, 0.f);

			for (auto const& enumValue : input.getActiveInputs())
			{
				switch (enumValue)
				{
					//TODO
					case(UserInputActionsEnum::PRESSING_W):
					{
						position += glm::vec3(0.f,0.f, .005f);
						rotation += glm::vec3(0.f, -180.f, 0.f);
						break;
					}
					case(UserInputActionsEnum::PRESSING_S):
					{
						position += glm::vec3(0.f,0.f,-.005f);
						break;
					}
					case(UserInputActionsEnum::PRESSING_A):
					{
						position += glm::vec3(.005f, 0.f, 0.f);
						rotation += glm::vec3(0.f, -90.f, 0.f);
						break;
					}
					case(UserInputActionsEnum::PRESSING_D):
					{
						position += glm::vec3(-.005f, 0.f, 0.f);
						rotation += glm::vec3(0.f, 90.f, 0.f);
						break;
					}
					default:
					{
						break;
					}
				}
			}

			transform.setRotation(rotation);
			transform.setTranslation(position);
			*/
		}
	}

	static const void runRenderSystem(Scene& scene, Window& window)
	{

		//View matrix
		//TODO: make there be another view matrix for the ui
		glm::mat4 viewMatrix = glm::lookAt(
			glm::vec3(0.f,10.f,-20.f), 
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f)
		);

		// Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1024.f/768.f, 1.f, 100.f);

		for (auto const& entity : EntityFilter<TransformComponent, ModelComponent>(scene))
		{
			ModelComponent& model = scene.getComponent<ModelComponent>(entity);
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);

			//TODO: move the mvp matrix to a SSBO so we can do ... one ... draw
			//TODO: move smth else too TBD
			glm::mat4 modelMatrix = transform.getWorldMatrix();
			glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

			//Bind the model matrix via uniform
			window.getBoundShader().updateMat4Uniform("modelViewProjectionMatrix", mvp);

			//Draw the bits of the model
			window.draw(model.getVertices().size(), model.getIndices().size(), model.getVertices(), model.getIndices());
		}
	}
};

#endif