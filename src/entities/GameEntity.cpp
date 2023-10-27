#include "GameEntity.hpp"

GameEntity::GameEntity(int uid)
{
	m_uid = uid;
}

int GameEntity::getUID() const
{
	return m_uid;
}

GameEntity::~GameEntity()
{
	int i = 0;
}
