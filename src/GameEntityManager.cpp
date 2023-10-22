#include "GameEntityManager.hpp"

GameEntityManager::GameEntityManager()
{
}

int GameEntityManager::createEntity()
{
	int uid = m_currentUID;
	GameEntity entity(uid);
	m_gameEntities.push_back(entity);
	m_currentUID++;
	return uid;
}

std::vector<GameEntity>& GameEntityManager::getGameEntities()
{
	return m_gameEntities;
}
