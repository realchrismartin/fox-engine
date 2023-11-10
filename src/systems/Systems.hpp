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
		//Update each component's local and world matrices if needed.
		//Track whether an update was made by keeping track of the overall graph's dirtiness
		bool anyNodeWorldDirty = false;
		std::function<void(int, Scene&, TransformComponent&)> rootNodeFunctor = [&anyNodeWorldDirty](int entityUID, Scene& scene, TransformComponent& component)
		{
			component.updateLocalAndWorldMatrix();

			anyNodeWorldDirty = anyNodeWorldDirty || component.isWorldDirty();
		};

		std::function<void(int, Scene&, TransformComponent&,TransformComponent&)> childNodeFunctor = [&anyNodeWorldDirty](int entityUID, Scene& scene, TransformComponent& parentComponent, TransformComponent& childComponent)
		{
			childComponent.updateLocalAndWorldMatrix(parentComponent);

			anyNodeWorldDirty = anyNodeWorldDirty || childComponent.isWorldDirty();
		};

		scene.applyToSceneGraph<TransformComponent>(rootNodeFunctor,childNodeFunctor);

		//If the graph didn't update at all, stop here
		//TODO: not working
		if (!anyNodeWorldDirty)
		{
			return;
		}

		//If the graph updated, we can clear the world dirty flags now that all parent-child relationships have updated.
		std::function<void(int, Scene&, TransformComponent&)> rootCleanFunctor = [](int entityUID, Scene& scene, TransformComponent& component)
		{;
			component.markWorldClean();
		};

		std::function<void(int, Scene&, TransformComponent&,TransformComponent&)> childCleanFunctor = [](int entityUID, Scene& scene, TransformComponent& parentComponent, TransformComponent& childComponent)
		{
			childComponent.markWorldClean();
		};

		scene.applyToSceneGraph<TransformComponent>(rootCleanFunctor,childCleanFunctor);
	}

	static const void runInputProcessingSystem(Scene& scene, float elapsedTime)
	{
		//Move transforms around with the input
		for (auto const& entity : EntityFilter<InputComponent, TransformComponent>(scene))
		{
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
			InputComponent& input = scene.getComponent<InputComponent>(entity);

			if (input.anyInputActive())
			{
				transform.setRotation(input.getRotationGivenInput());
				transform.addTranslation(input.getTranslationGivenInput());
			}
		}
	}

	static const void runRenderSystem(Scene& scene, Window& window)
	{

		//View matrix
		//TODO: make there be another view matrix for the ui

		glm::vec3 cameraCenter = glm::vec3(0.f,10.f,-10.f);
		glm::vec3 cameraUpVector = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 eyeTarget = glm::vec3(0.f, 0.f, 0.f);

		std::optional<int> cameraEntityId = scene.getCameraEntity();

		if (cameraEntityId.has_value())
		{
			if (scene.hasComponent<TransformComponent>(cameraEntityId.value()))
			{
				TransformComponent& cameraTransform = scene.getComponent<TransformComponent>(cameraEntityId.value());
				
				glm::mat4 worldMatrix = cameraTransform.getWorldMatrix();
				glm::vec4 center = glm::vec4(0.f, 0.f, 0.f, 1.f);

				auto transformedCameraCenter = worldMatrix * center;

				cameraCenter = glm::vec3(transformedCameraCenter.x, transformedCameraCenter.y, transformedCameraCenter.z);
			}
		}

		std::optional<int> cameraTargetEntityId = scene.getCameraTargetEntity();

		if (cameraTargetEntityId.has_value())
		{
			if (scene.hasComponent<TransformComponent>(cameraTargetEntityId.value()))
			{
				TransformComponent& cameraTargetTransform = scene.getComponent<TransformComponent>(cameraTargetEntityId.value());
				
				glm::mat4 worldMatrix = cameraTargetTransform.getWorldMatrix();
				glm::vec4 target = glm::vec4(0.f, 0.f, 0.f, 1.f);

				auto transformedCameraTarget = worldMatrix * target;

				eyeTarget = glm::vec3(transformedCameraTarget.x, transformedCameraTarget.y, transformedCameraTarget.z);
			}
		}

		glm::mat4 viewMatrix = glm::lookAt(cameraCenter,eyeTarget, cameraUpVector);

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