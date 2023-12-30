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
	return m_scene.getEntity(m_entityIndex);
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
	int size = m_scene.getEntityCount();

	m_entityIndex++;

	//Skip all inactive entities and entities that don't have the specified components
	while (m_entityIndex < size && (!m_scene.isEntityAtIndexActive(m_entityIndex) || !m_scene.entityHasComponents(m_entityIndex,m_componentIds)))
	{
		m_entityIndex++;
	}

	return *this;
}

const EntityIterator EntityIterator::begin() const
{
	int index = 0;

	int size = m_scene.getEntityCount();

	//Skip all inactive entities and entities that don't have the specified components
	while (index < size && (!m_scene.isEntityAtIndexActive(index) || !m_scene.entityHasComponents(index, m_componentIds)))
	{
		index++;
	}

	return EntityIterator(m_scene, m_componentIds, index);
}

const EntityIterator EntityIterator::end() const
{
	//Return an iterator to the end, regardless
	return EntityIterator(m_scene,m_componentIds, m_scene.getEntityCount());
}
