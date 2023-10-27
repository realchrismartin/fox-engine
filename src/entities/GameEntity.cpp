#include "GameEntity.hpp"

GameEntity::GameEntity(int uid)
{
	m_uid = uid;
}

int GameEntity::getUID() const
{
	return m_uid;
}

bool GameEntity::hasComponent(int componentTypeId) const
{
	return m_componentTypeUIDs.count(componentTypeId);
}

bool GameEntity::hasAllComponents(std::vector<int>& components) const
{
	for (auto const& componentId : components)
	{
		if (!m_componentTypeUIDs.count(componentId))
		{
			return false;
		}
	}

	return true;
}

void GameEntity::registerComponent(int componentTypeUID)
{
	m_componentTypeUIDs.insert(componentTypeUID);
}

const std::set<int>& GameEntity::getComponentTypes() const
{
	return m_componentTypeUIDs;
}
