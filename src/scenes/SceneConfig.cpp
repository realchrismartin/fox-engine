#include "src/scenes/SceneConfig.hpp"

#include "src/entities/EntityInstanceConfig.hpp"

EntityInstanceConfig SceneConfig::addEntity(GameEntityEnum entity)
{
	m_entities.push_back(entity);
	EntityInstanceConfig configEntity;
	configEntity.entityIndex = m_entities.size() - 1;
	configEntity.config = this;
	return configEntity;
}

<<<<<<< HEAD
void SceneConfig::addInitFnForEntity(const EntityInstanceConfig& entity, const std::function<void(int,Scene&)>& initFn)
=======
void SceneConfig::addInitFnForEntity(size_t index, std::function<void(int,Scene&)> initFn)
>>>>>>> 3607be4 (shipit)
{
	m_sceneSpecificInitFnMap[entity.entityIndex] = initFn;
}

void SceneConfig::addChild(const EntityInstanceConfig& parent, const EntityInstanceConfig& child) 
{
	size_t parentIndex = parent.entityIndex;
	size_t childIndex = child.entityIndex;

	if (m_entities.size() < parentIndex || m_entities.size() < childIndex)
	{
		//Can't do this.
		Logger::log("Can't add scene graph child config, not enough configs in the list.");
		return;
	}

	if (m_existingChildren.count(childIndex))
	{
		Logger::log("Can't add a child that is already a child of another parent.");
		return;
	}

	m_existingChildren.insert(childIndex);

	if (m_sceneGraphConfig.count(parentIndex))
	{
		m_sceneGraphConfig.at(parentIndex).insert(childIndex);
	}
	else 
	{
		m_sceneGraphConfig[parentIndex] = { childIndex };
	}
}

const std::vector<GameEntityEnum>& SceneConfig::getGameEntities() const
{
	return m_entities;
}

const std::unordered_map<size_t, std::set<size_t>>& SceneConfig::getSceneGraphMap() const
{
	return m_sceneGraphConfig;
}

<<<<<<< HEAD
const std::unordered_map<size_t, std::function<void(int, Scene&)>>& SceneConfig::getSceneSpecificInitFnMap() const
=======
const std::unordered_map<int, std::function<void(int, Scene&)>>& SceneConfig::getSceneSpecificInitFnMap() const
>>>>>>> 3607be4 (shipit)
{
	return m_sceneSpecificInitFnMap;
}
