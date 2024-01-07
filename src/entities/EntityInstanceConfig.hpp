#ifndef ENTITYINSTANCECONFIG_HPP 
#define ENTITYINSTANCECONFIG_HPP 

#include "src/scenes/SceneConfig.hpp"

/// @brief Represents configuration of a specific instance of an entity "type"
/// @brief Returned by SceneConfig to allow for ergonomic creation of scenes in SceneLibrary
struct EntityInstanceConfig
{
	size_t entityIndex = 0;
	SceneConfig* config = nullptr;

	/// @brief Add a functor to the config that represents scene-specific "settings" for an entity
	/// @param initFn 
	void addInitFn(std::function<void(int,Scene&)> initFn)
	{
		if (config == nullptr)
		{
			return;
		}

		config->addInitFnForEntity(*this, initFn);
	}

	/// @brief Add another entity as a child of this entity in the scene's scene graph
	/// @param childEntity 
	void addChild(const EntityInstanceConfig& childEntity)
	{
		if (config == nullptr)
		{
			return;
		}

		config->addChild(*this, childEntity);
	}
};

#endif
