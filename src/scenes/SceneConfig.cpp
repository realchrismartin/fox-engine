#include "src/scenes/SceneConfig.hpp"

#include "src/entities/GameEntityConfig.hpp"
#include "src/entities/EntityInstanceConfig.hpp"

EntityInstanceConfig SceneConfig::addEntity(const GameEntityConfig& entityConfig)
{
	EntityInstanceConfig configEntity;
	configEntity.entityConfig = entityConfig;
	configEntity.entityUID = m_entities.size();
	m_entities.push_back(configEntity); //Keep a copy for ourselves.

	configEntity.sceneConfig = this;
	return configEntity; //Return a copy so we don't accidentally inconvenience ourselves due to vector realloc
}

void SceneConfig::addInitFnForEntity(const EntityInstanceConfig& entity, const std::function<void(int,Scene&)>& initFn)
{
	m_sceneSpecificInitFnMap[entity.entityUID] = initFn;
}

void SceneConfig::addChild(const EntityInstanceConfig& parent, const EntityInstanceConfig& child) 
{
	int parentUID = parent.entityUID;
	int childUID = child.entityUID;

	if (m_existingChildren.count(childUID))
	{
		Logger::log("Can't add a child that is already a child of another parent.");
		return;
	}

	m_existingChildren.insert(childUID);

	if (m_sceneGraphConfig.count(parentUID))
	{
		m_sceneGraphConfig.at(parentUID).insert(childUID);
	}
	else 
	{
		m_sceneGraphConfig[parentUID] = { childUID };
	}
}

const std::vector<EntityInstanceConfig>& SceneConfig::getGameEntities() const
{
	return m_entities;
}

const std::unordered_map<int, std::set<int>>& SceneConfig::getSceneGraphMap() const
{
	return m_sceneGraphConfig;
}

const std::unordered_map<int, std::function<void(int, Scene&)>>& SceneConfig::getSceneSpecificInitFnMap() const
{
	return m_sceneSpecificInitFnMap;
}
