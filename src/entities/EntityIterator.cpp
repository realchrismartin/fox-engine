#include "src/entities/EntityIterator.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/scenes/Scene.hpp"

EntityIterator::EntityIterator(Scene& scene, std::vector<int>& componentIds, int entityIndex) : m_scene(scene), m_componentIds(componentIds)
{
	m_entityIndex = entityIndex;
}

int EntityIterator::getEntityIndex() const
{
	return m_entityIndex;
}

GameEntity EntityIterator::operator*() const
{
	return m_scene.getEntities()[m_entityIndex];
}

bool EntityIterator::operator==(const EntityIterator& other) const
{
	return m_entityIndex == other.getEntityIndex();
}

bool EntityIterator::operator!=(const EntityIterator& other) const
{
	return m_entityIndex != other.getEntityIndex();
}

EntityIterator& EntityIterator::operator++()
{
	int size = (int)m_scene.getEntities().size();

	m_entityIndex++;

	//Keep advancing if the now-current entry is invalid and there's more items in the entity list
	while (m_entityIndex < size && !m_scene.entityHasComponents(m_entityIndex,m_componentIds))
	{
		m_entityIndex++;
	}

	return *this;
}

const EntityIterator EntityIterator::begin() const
{
	int index = 0;

	int size = (int)m_scene.getEntities().size();

	//Skip all of the entities that don't match at the beginning
	while (index < size && !m_scene.entityHasComponents(index,m_componentIds))
	{
		index++;
	}

	return EntityIterator(m_scene, m_componentIds, index);
}

const EntityIterator EntityIterator::end() const
{
	//Return an iterator to the end, regardless
	return EntityIterator(m_scene,m_componentIds, (int)m_scene.getEntities().size());
}
