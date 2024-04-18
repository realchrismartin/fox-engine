#ifndef SCENECONFIG_HPP
#define SCENECONFIG_HPP

#include "src/entities/EntityInstanceConfig.hpp"

struct GameEntityConfig;
class Scene;

struct SceneConfig
{
	EntityInstanceConfig addEntity(const GameEntityConfig& entityConfig);

	void addInitFnForEntity(size_t index, std::function<void(int, Scene&)> initFn);

	void addChild(const EntityInstanceConfig& parent, const EntityInstanceConfig& child);

	const std::vector<GameEntityEnum>& getGameEntities() const;

	const std::unordered_map<size_t, std::set<size_t>>& getSceneGraphMap() const;

	const std::unordered_map<size_t, std::function<void(int, Scene&)>>& getSceneSpecificInitFnMap() const;
	
private:
	std::vector<GameEntityEnum> m_entities;
	std::unordered_map<size_t, std::set<size_t>> m_sceneGraphConfig;
	std::unordered_map<size_t, std::function<void(int, Scene&)>> m_sceneSpecificInitFnMap;
	std::set<size_t> m_existingChildren;
	const std::unordered_map<int, std::function<void(int, Scene&)>>& getSceneSpecificInitFnMap() const;
	
private:
	std::vector<GameEntityEnum> m_entities;
	std::unordered_map<int, std::set<int>> m_sceneGraphConfig;
	std::unordered_map<int, std::function<void(int, Scene&)>> m_sceneSpecificInitFnMap;
	std::set<int> m_existingChildren;
};

#endif
