#ifndef GAMEENTITYLIBRARY_HPP
#define GAMEENTITYLIBRARY_HPP

enum class GameEntityEnum;
struct GameEntityConfig;

//To add a new game entity:
// 0) Update GameEntityEnum.hpp to add an ID for your game entity
// 1) Add a static GameEntityConfig variable for your enum to the GameEntities namespace in GameEntityLibrary.cpp
// 2) Add a mapping in getGameEntityConfig() for your enum to the variable created in #1

namespace GameEntityLibrary
{
	/// @brief Get a reference to the static game entity config for a given entity type
	/// @param gameEntity 
	/// @return 
	const GameEntityConfig& getGameEntityConfig(GameEntityEnum gameEntity);
}

#endif