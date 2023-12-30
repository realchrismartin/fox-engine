#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "src/scenes/Scene.hpp"
#include "src/scenes/SceneEnum.hpp"
#include "src/scenes/SceneLibrary.hpp"
#include "src/util/Logger.hpp"

#include "src/graphics/Window.hpp"
#include "src/graphics/Camera.hpp"
#include "src/Clock.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/ModelComponent.hpp"
#include "src/components/MVPTransformComponent.hpp"

#include "src/systems/MessageRelay.hpp"
#include "src/systems/MessageTypes.hpp"

static const float TIMESTEP = .0167f;

/// @brief A collection of static functions that are "systems", functions that operate on specific associations of components in a scene to update them.
/// @brief The update and render meta-systems are the core of the game. 
class Systems
{
public:

	static const void runGame()
	{
		//Try to initialize our windowing library.
		//If we can't, bomb out here.
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			SDL_Log("SDL_Init_VIDEO failed (%s)", SDL_GetError());
			SDL_Quit();
			return;
		}

		//Initialize the game elements on the stack now. 
		Clock clock;
		Window window;
		Camera camera;

		//Initialize the scene as the main menu.
		Scene scene(SceneLibrary::getSceneConfig(SceneEnum::MAIN_MENU));

		float currentTime = clock.getElapsedTimeInSeconds();
		float accumulator = TIMESTEP;

		//This is the main game loop.
		while (window.isOpen())
		{
			float newTime = clock.getElapsedTimeInSeconds();

			float frameTime = newTime - currentTime;

			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= TIMESTEP)
			{
				update(window, scene, camera, TIMESTEP);
				accumulator -= TIMESTEP;
			}

			render(window, scene, camera);
		}

		Logger::log("See you next time, space fox boy...");
	}

private:

	//Run all of the game systems that pertain to updating
	static const void update(Window& window, Scene& scene, Camera& camera, float elapsedTime)
	{
		pollEventSystem(window, scene, camera);
		updateCameraProjectionSystem(window, camera);
		runInputProcessingSystem(scene, elapsedTime);
		runAnimationSystem(scene, elapsedTime);
		runSceneGraphUpdateSystem(scene, camera);
		cleanDirtyFlagsSystem(window, scene, camera);
	};

	//Run all of the game systems that pertain to rendering
	//Assumes we called update first.
	static const void render(Window& window, Scene& scene, Camera& camera)
	{
		window.clear();

		//Draw stuff to the window
		runRenderSystem(window, scene, camera);

		window.display();
	};


	static const void updateCameraProjectionSystem(const Window& window, Camera& camera)
	{
		if (!window.isWindowSizeDirty())
		{
			return;
		}

		glm::i64vec2 windowSize = window.getWindowSize();
		camera.updateOrthographicProjectionMatrix(windowSize);
		camera.updatePerspectiveProjectionMatrix(windowSize);
	}

	static const void pollEventSystem(Window& window, Scene& scene, Camera& camera)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			//Inform the window of window events
			//TODO: inform the camera if the window size changes
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED || event.type == SDL_EVENT_QUIT)
			{
				window.close();
				break;
			}

			//If the window is resized by a user, ensure that the camera will update its projection matrices, which depend on the size of the window
			//TODO: this is not tested
			if (event.type == SDL_EVENT_WINDOW_RESIZED)
			{
				window.markWindowSizeDirty();
			}

			//Inform the input components of input events
			//TODO: add mouse events
			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
			{
				for (auto const& entity : EntityFilter<InputComponent>(scene))
				{
					scene.getComponent<InputComponent>(entity).informOfEvent(event);
				}

				//Also inform the scene if a change is needed, for now
				//NOTE: This will request a reload even if the current scene is the same. Fix this later.

				//For now, 0 will load the menu
				if (event.key.keysym.scancode == SDL_SCANCODE_0)
				{
					SceneChangeMessage message;
					message.requestedScene = SceneEnum::MAIN_MENU;
					MessageRelay::getInstance()->sendMessage(message);
				}

				//1 will load the level 1
				if (event.key.keysym.scancode == SDL_SCANCODE_1)
				{
					SceneChangeMessage message;
					message.requestedScene = SceneEnum::LEVEL_1;
					MessageRelay::getInstance()->sendMessage(message);
				}
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
		std::function<void(Scene&, std::optional<int>,int)> functor = [&camera](Scene& scene, std::optional<int> parentEntityUID, int entityUID)
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
		};

		//Update the whole scene graph - any entity that has transforms will have those transforms updated.
		scene.applyToSceneGraph(functor);

		//Now that we've updated world matrices and marked them dirty, the camera may want to update its view, since the scene may have specified a camera perspective.
		//Update the camera view matrix accordingly if needed
		camera.updateViewMatrix(scene);

		//Now that we've updated both the world matrices and view matrices, update the MVP matrices
		//Unfortunately we can't do this at the same time as the scene graph traversal b/c the camera view can depend on the world matrices of the targets,
		//so we waste some efficiency by doing another iteration and component lookup...
		for (auto const& entity : EntityFilter<ModelComponent,TransformComponent, MVPTransformComponent>(scene))
		{
			ModelComponent& model = scene.getComponent<ModelComponent>(entity);
			TransformComponent& transform = scene.getComponent<TransformComponent>(entity);
			MVPTransformComponent& mvpTransform = scene.getComponent<MVPTransformComponent>(entity);

			//If any of the three matrices is dirty, update the combined MVP matrix.
			if (model.usesOrthographicProjection() && (transform.isWorldMatrixDirty() || camera.isViewMatrixDirty() || camera.isOrthographicProjectionMatrixDirty()))
			{
				mvpTransform.setMVPMatrix(transform.getWorldMatrix(), glm::mat4(1.0), camera.getOrthographicProjectionMatrix());
			}
			else if (!model.usesOrthographicProjection() && (transform.isWorldMatrixDirty() || camera.isViewMatrixDirty() || camera.isPerspectiveProjectionMatrixDirty()))
			{
				mvpTransform.setMVPMatrix(transform.getWorldMatrix(), camera.getViewMatrix(), camera.getPerspectiveProjectionMatrix());
			}
		}
	}

	static const void cleanDirtyFlagsSystem(Window& window, Scene& scene, Camera& camera)
	{
		//Mark the window size clean
		window.markWindowSizeClean();

		//Mark the camera transform dirty flag clean
		camera.markViewMatrixClean();

		//We can clear the world dirty flags now that all parent-child relationships have updated.
		std::function<void(Scene&, std::optional<int>,int)> cleanFunctor = [](Scene& scene, std::optional<int> parentEntityUID, int entityUID)
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
			if (!scene.isEntityActive(entity))
			{
				//Skip including these indices and vertices. Note that the MVPs will still all be copied over.
				continue;
			}

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
		//NB: as above, we send all of the MVP data even if we're not drawing everything in the scene
		window.draw(vertices.size(), indices.size(), mvpTransformComponentPool.getComponentsInUse(), &vertices[0], &indices[0], (GLvoid*)mvpTransformComponentPool.getData());
	}
};

#endif