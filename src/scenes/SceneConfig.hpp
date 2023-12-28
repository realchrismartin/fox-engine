#ifndef SCENECONFIG_HPP
#define SCENECONFIG_HPP

#include "src/entities/GameEntityEnum.hpp"
#include "src/scenes/SceneEnum.hpp"

struct ConfiguredEntity;
class Scene;
class GameEntity;

struct SceneConfig
{
	SceneEnum id = SceneEnum::NONE;

	ConfiguredEntity addEntity(GameEntityEnum entity);

	void addInitFnForEntity(size_t index, std::function<void(const GameEntity&, Scene&)> initFn);

	void addChild(const ConfiguredEntity& parent, const ConfiguredEntity& child);

	const std::vector<GameEntityEnum>& getGameEntities() const;

	const std::unordered_map<int, std::set<int>>& getSceneGraphMap() const;

	const std::unordered_map<int, std::function<void(const GameEntity&, Scene&)>>& getSceneSpecificInitFnMap() const;
	
private:
	std::vector<GameEntityEnum> m_entities;
	std::unordered_map<int, std::set<int>> m_sceneGraphConfig;
	std::unordered_map<int, std::function<void(const GameEntity&, Scene&)>> m_sceneSpecificInitFnMap;
	std::set<int> m_existingChildren;
};

#endif
