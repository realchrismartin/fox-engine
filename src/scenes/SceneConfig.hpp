#ifndef SCENECONFIG_HPP
#define SCENECONFIG_HPP

#include "src/entities/GameEntityLibrary.hpp"

struct SceneConfig
{
	SceneConfig addEntity(GameEntityEnum entity)
	{
		m_entities.push_back(entity);
		return *this;
	}

	SceneConfig addEntity(GameEntityEnum entity, std::function<void(const GameEntity&,Scene&)> withSpecificInitFn)
	{
		m_entities.push_back(entity);
		m_sceneSpecificInitFnMap[(int)m_entities.size() - 1] = withSpecificInitFn;
		return *this;
	}

	SceneConfig addChild(int parentIndex, int childIndex) 
	{
		if (m_entities.size() < parentIndex || m_entities.size() < childIndex)
		{
			//Can't do this.
			Logger::log("Can't add scene graph child config, not enough configs in the list.");
			return *this;
		}

		if (m_existingChildren.count(childIndex))
		{
			Logger::log("Can't add a child that is already a child of another parent.");
			return *this;
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

		return *this;
	}

	const std::vector<GameEntityEnum>& getGameEntities() const
	{
		return m_entities;
	}

	const std::unordered_map<int, std::set<int>>& getSceneGraphMap() const
	{
		return m_sceneGraphConfig;
	}

	const std::unordered_map<int, std::function<void(const GameEntity&, Scene&)>>& getSceneSpecificInitFnMap() const
	{
		return m_sceneSpecificInitFnMap;
	}
	
private:
	std::vector<GameEntityEnum> m_entities;
	std::unordered_map<int, std::set<int>> m_sceneGraphConfig;
	std::unordered_map<int, std::function<void(const GameEntity&, Scene&)>> m_sceneSpecificInitFnMap;
	std::set<int> m_existingChildren;
};

#endif
