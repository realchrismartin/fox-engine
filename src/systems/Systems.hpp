#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/Window.hpp"
#include "src/scenes/Scene.hpp"
#include "src/graphics/Camera.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/VerticesComponent.hpp"
#include "src/components/ModelComponent.hpp"
#include "src/components/WorldTransformComponent.hpp"

#include "src/graphics/MVPMatrix.hpp"

/// @brief A collection of static functions that are "systems", functions that operate on specific associations of components in a scene to update them.
/// @brief The update and render meta-systems are the core of the game. 
class Systems
{
public:
	//Run all of the game systems that pertain to updating
	static const void update(Window& window, Scene& scene, Camera& camera, float elapsedTime)
	{
		runWindowEventSystem(window, scene);
		runSceneGraphUpdateSystem(scene);
		runInputProcessingSystem(scene, elapsedTime);
	};

	//Run all of the game systems that pertain to rendering
	//Assumes we called update first.
	static const void render(Window& window, Scene& scene, Camera& camera)
	{
		//Clear the window.
		window.clear();

		//Draw stuff to the window
		runRenderSystem(window, scene, camera);

		//Display whatever has been drawn to the window since the clear happened, i.e. all of the stuff that was drawn during update.
		window.display();
	};
private:
	static const void runWindowEventSystem(Window& window, Scene& scene)
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
		//NB: we track the world matrix in a sep. component so we can upload the whole pool to the gpu at once
		bool anyNodeWorldDirty = false;
		std::function<void(Scene&, std::optional<int>,int)> functor = [&anyNodeWorldDirty](Scene& scene, std::optional<int> parentEntityUID, int entityUID)
		{
			if (!scene.hasComponent<TransformComponent>(entityUID))
			{
				return;
			}

			if (!scene.hasComponent<WorldTransformComponent>(entityUID))
			{
				return;
			}

			TransformComponent& transform = scene.getComponent<TransformComponent>(entityUID);
			WorldTransformComponent& worldTransform = scene.getComponent<WorldTransformComponent>(entityUID);

			//If this is a root node, update it directly and return.
			if (!parentEntityUID.has_value())
			{
				//Update the transform and world transform locally
				transform.updateLocalAndWorldMatrix(worldTransform);
				anyNodeWorldDirty = anyNodeWorldDirty || transform.isWorldMatrixDirty();
				return;
			}

			//If this is a child node, update versus the parent.
			if (!scene.hasComponent<TransformComponent>(parentEntityUID.value()))
			{
				Logger::log("For some reason this child node has a transform, but its parent doesn't! This is probably a bug.");
				return;
			}

			TransformComponent& parentTransform = scene.getComponent<TransformComponent>(parentEntityUID.value());
			WorldTransformComponent& parentWorldTransform = scene.getComponent<WorldTransformComponent>(parentEntityUID.value());

			//Update the transform and the world transform using the parent transform
			transform.updateLocalAndWorldMatrix(parentTransform,parentWorldTransform, worldTransform);
			anyNodeWorldDirty = anyNodeWorldDirty || parentTransform.isWorldMatrixDirty();
		};

		scene.applyToSceneGraph(functor);

		//If the graph didn't update at all, stop here
		if (!anyNodeWorldDirty)
		{
			return;
		}

		//If the graph updated, we can clear the world dirty flags now that all parent-child relationships have updated.
		std::function<void(Scene&, std::optional<int>,int)> cleanFunctor = [&anyNodeWorldDirty](Scene& scene, std::optional<int> parentEntityUID, int entityUID)
		{
			//Since this functor runs on all nodes, ignore the parent and mark the current node clean.
			if (!scene.hasComponent<TransformComponent>(entityUID))
			{
				return;
			}

			TransformComponent& component = scene.getComponent<TransformComponent>(entityUID);
			component.markWorldMatrixClean();
		};


		scene.applyToSceneGraph(cleanFunctor);
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

	static const void runRenderSystem(Window& window, Scene& scene, Camera& camera)
	{
		//Calculate the combined projection-view matrix and send it to the GPU as a uniform.
		//TODO: later, we will want a separate view matrix for the UI. Probably pass two uniforms and another VBO element.
		glm::mat4 viewMatrix = camera.getViewMatrix(scene);
		glm::mat4 projectionMatrix = camera.getProjectionMatrix();
		glm::mat4 projectionViewMatrix = projectionMatrix * viewMatrix;

		window.getBoundShader().bind();
		window.getBoundShader().updateMat4Uniform("projectionViewMatrix", projectionViewMatrix);
		window.getBoundShader().unbind();

		ComponentPool& modelComponentPool = scene.getComponentPool<ModelComponent>();
		ComponentPool& verticesComponentPool = scene.getComponentPool<VerticesComponent>();
		ComponentPool& worldTransformComponentPool = scene.getComponentPool<WorldTransformComponent>();

		//Upstream, we guaranteed that if an entity has a model, it has a transform, vertices, and indices
		//This means we can iterate over the model pool and it will be in the correct / same order as the other pools
		//It would be ideal to not have to rebuild this list constantly, but we are not allowed to have gaps in the indices list.
		std::vector<GLuint> indices;
		for (auto const& entity : modelComponentPool.getRegisteredEntityUIDs())
		{
			ModelComponent& model = scene.getComponent<ModelComponent>(entity);
			indices.insert(indices.end(), model.getIndices().begin(), model.getIndices().end());
		}

		window.draw(verticesComponentPool.getComponentsInUse() * VerticesComponent::MAX_VERTICES, indices.size(), verticesComponentPool.getComponentsInUse(), (GLvoid*)verticesComponentPool.getData(), &indices[0], (GLvoid*)worldTransformComponentPool.getData());
	}
};

#endif