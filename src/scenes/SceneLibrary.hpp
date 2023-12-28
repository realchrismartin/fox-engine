#ifndef SCENELIBRARY_HPP
#define SCENELIBRARY_HPP

enum class SceneEnum;
struct SceneConfig;

//To add a new scene:
// 0) Update SceneEnum.hpp to add an ID for your scene
// 1) Add a mapping in getSceneConfig() for your enum (below)
// 2) Add a mapping and definition in initSceneConfig() for your enum (below)
namespace SceneLibrary
{
	/// @brief Get a statically allocated scene configuration. This will be called from Scene:: when a new scene is requested.
	/// @brief It will always return a reference to the single statically allocated config for the scene enum specified.
	/// @param scene 
	/// @return 
	const SceneConfig& getSceneConfig(SceneEnum scene);

	/// @brief Build a scene config for the scene. This should not be called directly except when statically initializing scenes.
	/// @brief Define your scene's contents here!
	/// @param scene 
	/// @return 
	const SceneConfig initSceneConfig(SceneEnum scene);
}

#endif
