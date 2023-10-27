#ifndef ENTITYITERATOR_HPP
#define ENTITYITERATOR_HPP

class Scene;
class GameEntity;

/// @brief An iterator that is used by EntityFilter, will iterate over the entities in a scene but only iterate over entities with the specified componentIds.
class EntityIterator
{
public:
	EntityIterator(Scene& scene, std::vector<int>& componentIds, int entityIndex);
	int getEntityIndex() const;
	GameEntity operator*() const;
	bool operator==(const EntityIterator& other) const;
	bool operator!=(const EntityIterator& other) const;
	EntityIterator& operator++();
	const EntityIterator begin() const;
	const EntityIterator end() const;
private:
	int m_entityIndex = 0;
	Scene& m_scene;
	std::vector<int>& m_componentIds;
};

#endif
