#ifndef SCENECONFIG_HPP
#define SCENECONFIG_HPP

#include "src/entities/GameEntityEnum.hpp"
#include "src/scenes/SceneEnum.hpp"

struct EntityInstanceConfig;
class Scene;
class GameEntity;

struct SceneConfig
{
	SceneEnum id = SceneEnum::NONE;

	EntityInstanceConfig addEntity(GameEntityEnum entity);

	void addInitFnForEntity(const EntityInstanceConfig& entity, const std::function<void(int, Scene&)>& initFn);

	void addChild(const EntityInstanceConfig& parent, const EntityInstanceConfig& child);

	const std::vector<GameEntityEnum>& getGameEntities() const;

	const std::unordered_map<size_t, std::set<size_t>>& getSceneGraphMap() const;

	const std::unordered_map<size_t, std::function<void(int, Scene&)>>& getSceneSpecificInitFnMap() const;
	
private:
	std::vector<GameEntityEnum> m_entities;
	std::unordered_map<size_t, std::set<size_t>> m_sceneGraphConfig;
	std::unordered_map<size_t, std::function<void(int, Scene&)>> m_sceneSpecificInitFnMap;
	std::set<size_t> m_existingChildren;
};

#endif
