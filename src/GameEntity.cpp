#include "GameEntity.hpp"

GameEntity::GameEntity(int uid)
{
	m_uid = uid;
}

int GameEntity::getUID() const
{
	return m_uid;
}

bool GameEntity::hasComponent(int componentTypeUID) const
{
	return m_componentTypeUIDs.count(componentTypeUID);
}

void GameEntity::registerComponent(int componentTypeUID)
{
	m_componentTypeUIDs.insert(componentTypeUID);
}
