#ifndef ENTITYINSTANCECONFIG_HPP 
#define ENTITYINSTANCECONFIG_HPP 

<<<<<<< HEAD
#include "src/scenes/SceneConfig.hpp"
=======
#include "src/entities/GameEntityConfig.hpp"

struct SceneConfig;
>>>>>>> 94c3281 (get ready)

/// @brief Represents configuration of a specific instance of an entity "type"
/// @brief Returned by SceneConfig to allow for ergonomic creation of scenes in SceneLibrary
struct EntityInstanceConfig
{
<<<<<<< HEAD
	size_t entityIndex = 0;
	SceneConfig* config = nullptr;
=======
	int entityUID = 0;
	SceneConfig* sceneConfig = nullptr;
	GameEntityConfig entityConfig;
>>>>>>> 94c3281 (get ready)

	/// @brief Add a functor to the config that represents scene-specific "settings" for an entity
	/// @param initFn 
	void addInitFn(std::function<void(int,Scene&)> initFn);

	/// @brief Add another entity as a child of this entity in the scene's scene graph
	/// @param childEntity 
	void addChild(const EntityInstanceConfig& childEntity);
};

#endif
