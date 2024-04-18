#ifndef SCENECONFIG_HPP
#define SCENECONFIG_HPP

#include "src/entities/EntityInstanceConfig.hpp"

struct GameEntityConfig;
class Scene;

struct SceneConfig
{
	EntityInstanceConfig addEntity(const GameEntityConfig& entityConfig);

	void addInitFnForEntity(const EntityInstanceConfig& entity, const std::function<void(int, Scene&)>& initFn);

	void addChild(const EntityInstanceConfig& parent, const EntityInstanceConfig& child);

	const std::vector<EntityInstanceConfig>& getGameEntities() const;

	const std::unordered_map<int, std::set<int>>& getSceneGraphMap() const;

	const std::unordered_map<int, std::function<void(int, Scene&)>>& getSceneSpecificInitFnMap() const;
	
private:
	std::vector<EntityInstanceConfig> m_entities;
	std::unordered_map<int, std::set<int>> m_sceneGraphConfig;
	std::unordered_map<int, std::function<void(int, Scene&)>> m_sceneSpecificInitFnMap;
	std::set<int> m_existingChildren;
};

#endif
