#ifndef GAMEENTITYMANAGER_HPP
#define GAMEENTITYMANAGER_HPP

#include <vector>
#include "src/GameEntity.hpp"

class GameEntityManager
{
public:
	GameEntityManager();
	int createEntity();
	std::vector<GameEntity>& getGameEntities();
private:
	std::vector<GameEntity> m_gameEntities;
	int m_currentUID = 0;
};

#endif