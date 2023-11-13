#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/Window.hpp"
#include "src/scenes/Scene.hpp"
#include "src/graphics/Camera.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"
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
		runInputProcessingSystem(scene, elapsedTime);
		runAnimationSystem(scene, elapsedTime);
		runSceneGraphUpdateSystem(scene, camera);
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

	static const void runAnimationSystem(Scene& scene, float elapsedTime)
	{
		//World's dumbest animation system: just constantly cycle meshes at warp speed
		for (auto const& entity : EntityFilter<ModelComponent>(scene))
		{
			ModelComponent& model = scene.getComponent<ModelComponent>(entity);

			size_t numMeshes = model.getNumMeshes();

			if (numMeshes <= 1)
			{
				continue;
			}

			size_t nextMesh = model.getActiveMeshIndex() + 1;

			if (nextMesh < numMeshes)
			{
				model.setActiveMesh(nextMesh);
			}
			else 
			{
				model.setActiveMesh(0);
			}
		}
	}

	static const void runSceneGraphUpdateSystem(Scene& scene, Camera& camera)
	{
		//Update each component's local and world matrices if needed. If they change during this update, the world matrix will be marked dirty.
		//We also track if any node is dirty at all, since we need to reset the graph's dirty flags afterward if this is the case.
		bool anyNodeWorldDirty = false;

		std::function<void(Scene&, std::optional<int>,int)> functor = [&anyNodeWorldDirty,&camera](Scene& scene, std::optional<int> parentEntityUID, int entityUID)
		{
			if (!scene.hasComponent<TransformComponent>(entityUID))
			{
				return;
			}

			if (!scene.hasComponent<MVPTransformComponent>(entityUID))
			{
				return;
			}

			TransformComponent& transform = scene.getComponent<TransformComponent>(entityUID);
			MVPTransformComponent& mvpTransform = scene.getComponent<MVPTransformComponent>(entityUID);

			if (!parentEntityUID.has_value())
			{
				//If this is a root node, update it directly
				transform.updateLocalAndWorldMatrix();
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

				//Update the transform and the world transform using the parent transform
				transform.updateLocalAndWorldMatrix(parentTransform);
			}
			
			//track whether any node's world matrix is dirty overall so we can reset the graph's dirty flags after updating.
			anyNodeWorldDirty = anyNodeWorldDirty || transform.isWorldMatrixDirty();
		};

		//Update the whole scene graph - any entity that has transforms will have those transforms updated.
		scene.applyToSceneGraph(functor);

		//Now that we've updated world matrices and marked them dirty, update the camera matrices.
		//Update the camera matrices if needed. If they change during this update, they will be marked dirty.
		camera.updateProjectionMatrix();
		camera.updateViewMatrix(scene);

		//Now that we've updated both the world matrices and v/p matrices, update the MVP matrices
		//Unfortunately we can't do this at the same time as the scene graph traversal b/c the camera view can depend on the world matrices of the targets,
		//so we waste some efficiency by doing another iteration and component lookup...
		for (auto const& entity : EntityFilter<TransformComponent, MVPTransformComponent>(scene))
		{
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
			MVPTransformComponent& mvpTransform = scene.getComponent<MVPTransformComponent>(entity);

			bool modelDirty = transform.isWorldMatrixDirty();
			bool viewDirty = camera.isViewMatrixDirty();
			bool projectionDirty = camera.isProjectionMatrixDirty();

			//If any of the three matrices is dirty, update the combined MVP matrix.
			if (modelDirty || viewDirty || projectionDirty)
			{
				mvpTransform.setMVPMatrix(transform.getWorldMatrix(), camera.getViewMatrix(), camera.getProjectionMatrix());
			}
		}

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

		//Do the cleaning.
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
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		//Get a ref to the model component pool so we can iterate over the entities that have models and move their data to the buffer in the correct order.
		//Upstream, we guaranteed that if an entity has a model, it has a transform
		//This means we can iterate over the model pool and it will be in the correct / same order as the other pools
		//We use this property of the pools to guarantee that the MVPTransform ordering matches the vertices ordering when we updated the mvp indices earlier.
		ComponentPool& modelComponentPool = scene.getComponentPool<ModelComponent>();

		//Get a ref to the mvp transform pool so we can send the entire pool data to the GPU
		ComponentPool& mvpTransformComponentPool = scene.getComponentPool<MVPTransformComponent>();

		auto entityIds = modelComponentPool.getRegisteredEntityUIDs();
		for (auto const& entity : entityIds)
		{
			ModelComponent& model = scene.getComponent<ModelComponent>(entity);

			//It would be ideal to not have to rebuild this list constantly, but we are not allowed to have gaps in the indices list, and we don't like gaps in the vertices either.
			//Also, the index position changes because the meshes don't all have the same vertex count :(
			for (auto index : model.getIndices())
			{
				indices.push_back(index + (GLuint)vertices.size());
			}

			//Copy the vertex data to a temporary vector to remove the gaps
			vertices.insert(vertices.end(), model.getVertices().begin(), model.getVertices().end());
		}

		//Send vertices, indices, and MVPs, then draw them.
		window.draw(vertices.size(), indices.size(), mvpTransformComponentPool.getComponentsInUse(), &vertices[0], &indices[0], (GLvoid*)mvpTransformComponentPool.getData());
	}
};

#endif