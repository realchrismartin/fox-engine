#include "src/scenes/SceneConfig.hpp"

#include "src/entities/ConfiguredEntity.hpp"

ConfiguredEntity SceneConfig::addEntity(GameEntityEnum entity)
{
	m_entities.push_back(entity);
	ConfiguredEntity configEntity;
	configEntity.entityIndex = m_entities.size() - 1;
	configEntity.config = this;
	return configEntity;
}

void SceneConfig::addInitFnForEntity(size_t index, std::function<void(const GameEntity&,Scene&)> initFn)
{
	m_sceneSpecificInitFnMap[index] = initFn;
}

void SceneConfig::addChild(const ConfiguredEntity& parent, const ConfiguredEntity& child) 
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
		m_sceneGraphConfig[parentIndex] = { (int)childIndex };
	}
}

const std::vector<GameEntityEnum>& SceneConfig::getGameEntities() const
{
	return m_entities;
}

const std::unordered_map<int, std::set<int>>& SceneConfig::getSceneGraphMap() const
{
	return m_sceneGraphConfig;
}

const std::unordered_map<int, std::function<void(const GameEntity&, Scene&)>>& SceneConfig::getSceneSpecificInitFnMap() const
{
	return m_sceneSpecificInitFnMap;
}
