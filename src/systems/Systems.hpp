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
#include "src/components/MVPTransformComponent.hpp"

/// @brief A collection of static functions that are "systems", functions that operate on specific associations of components in a scene to update them.
/// @brief The update and render meta-systems are the core of the game. 
class Systems
{
public:
	//Run all of the game systems that pertain to updating
	static const void update(Window& window, Scene& scene, Camera& camera, float elapsedTime)
	{
		runWindowEventSystem(window, scene);
		runSceneGraphUpdateSystem(scene, camera);
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

	static const void runSceneGraphUpdateSystem(Scene& scene, Camera& camera)
	{
		
		//Update the camera matrices. If they changed, they will be marked dirty.
		camera.updateProjectionMatrix();
		camera.updateViewMatrix(scene);

		//Update each component's local and world matrices if needed.
		//Track whether an update was made by keeping track of the overall graph's dirtiness
		//NB: we track the world matrix in a sep. component so we can upload the whole pool to the gpu at once
		bool anyNodeWorldDirty = false;
		std::function<void(Scene&, std::optional<int>,int)> functor = [&anyNodeWorldDirty,&camera](Scene& scene, std::optional<int> parentEntityUID, int entityUID)
		{
			if (!scene.hasComponent<TransformComponent>(entityUID))
			{
				return;
			}

			if (!scene.hasComponent<WorldTransformComponent>(entityUID))
			{
				return;
			}

			if (!scene.hasComponent<MVPTransformComponent>(entityUID))
			{
				return;
			}

			TransformComponent& transform = scene.getComponent<TransformComponent>(entityUID);
			WorldTransformComponent& worldTransform = scene.getComponent<WorldTransformComponent>(entityUID);
			MVPTransformComponent& mvpTransform = scene.getComponent<MVPTransformComponent>(entityUID);

			if (!parentEntityUID.has_value())
			{
				//If this is a root node, update it directly
				transform.updateLocalAndWorldMatrix(worldTransform);
			}
			else 
			{
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
			}
			
			bool modelDirty = transform.isWorldMatrixDirty();
			bool viewDirty = camera.isViewMatrixDirty();
			bool projectionDirty = camera.isProjectionMatrixDirty();

			//If any of the three matrices is dirty, update the combined MVP matrix.
			if (modelDirty || viewDirty || projectionDirty)
			{
				mvpTransform.setMVPMatrix(worldTransform.getWorldMatrix(), camera.getViewMatrix(), camera.getProjectionMatrix());
			}

			//track whether any node's world matrix is dirty overall so we can reset the graph's dirty flags after updating.
			anyNodeWorldDirty = anyNodeWorldDirty || transform.isWorldMatrixDirty();
		};

		scene.applyToSceneGraph(functor);

		//Mark the camera transform dirty flags clean
		camera.markViewMatrixClean();
		camera.markProjectionMatrixClean();

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
		camera.updateViewMatrix(scene);

		glm::mat4 viewMatrix = camera.getViewMatrix();
		glm::mat4 projectionMatrix = camera.getProjectionMatrix();
		glm::mat4 projectionViewMatrix = projectionMatrix * viewMatrix;

		window.getBoundShader().bind();
		window.getBoundShader().updateMat4Uniform("projectionViewMatrix", projectionViewMatrix);
		window.getBoundShader().unbind();

		ComponentPool& modelComponentPool = scene.getComponentPool<ModelComponent>();
		ComponentPool& verticesComponentPool = scene.getComponentPool<VerticesComponent>();
		ComponentPool& worldTransformComponentPool = scene.getComponentPool<WorldTransformComponent>();
		ComponentPool& mvpTransformComponentPool = scene.getComponentPool<MVPTransformComponent>();

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		for (auto const& entity : modelComponentPool.getRegisteredEntityUIDs())
		{
			//Upstream, we guaranteed that if an entity has a model, it has a transform, vertices, and indices
			//This means we can iterate over the model pool and it will be in the correct / same order as the other pools
			//We use this property of the pools to guarantee that the WorldTransform ordering matches the vertices ordering when we updated the mvp indices earlier.
			
			//We also know that each entity with a model component has a vertex component, transform component, etc.

			ModelComponent& model = scene.getComponent<ModelComponent>(entity);
			VerticesComponent& verticesComponent = scene.getComponent<VerticesComponent>(entity);

			//Copy the index data to a temporary vector to remove the gaps
			//It would be ideal to not have to rebuild this list constantly, but we are not allowed to have gaps in the indices list.
			indices.insert(indices.end(), model.getIndices().begin(), model.getIndices().end());

			//Copy the vertex data to a temporary vector to remove the gaps
			//As much as I want to not do this, I just can't be OK with sending meaningless dead vertices to the GPU either.
			//TODO: stop doing this once we dynamically resize the vertices component pool and have (mostly) similar meshes?
			Vertex* verticesArray = (Vertex*)verticesComponent.getVertices();
			for (size_t i = 0; i < model.getVertexCount(); i++)
			{
				vertices.push_back(verticesArray[i]);
			}
		}

		window.draw(vertices.size(), indices.size(), mvpTransformComponentPool.getComponentsInUse(), &vertices[0], &indices[0], (GLvoid*)mvpTransformComponentPool.getData());
		//window.draw(vertices.size(), indices.size(), worldTransformComponentPool.getComponentsInUse(), &vertices[0], &indices[0], (GLvoid*)worldTransformComponentPool.getData());
	}
};

#endif