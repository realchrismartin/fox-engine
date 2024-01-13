#include "src/entities/EntityIterator.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/scenes/Scene.hpp"

EntityIterator::EntityIterator(Scene& scene, std::vector<int>& componentIds, size_t entityIndex) : m_scene(scene), m_componentIds(componentIds)
{
	m_entityIndex = entityIndex;
}

size_t EntityIterator::getEntityIndex() const
{
	return m_entityIndex;
}

int EntityIterator::operator*() const
{
	return m_scene.getEntityUIDForIndex(m_entityIndex).value(); //Assumes that the ++ and begin() methods will skip entities that don't exist
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
	size_t size = m_scene.getEntityCount();

	m_entityIndex++;

	//Skip entities that don't have the specified components
	//NB: entityExists is used so that we can operator* without checking the value exists
	while (m_entityIndex < size && (!m_scene.entityExists(m_entityIndex) || !m_scene.entityHasComponents(m_entityIndex,m_componentIds)))
	{
		m_entityIndex++;
	}

	return *this;
}

const EntityIterator EntityIterator::begin() const
{
	size_t index = 0;

	size_t size = m_scene.getEntityCount();

	//Skip entities that don't have the specified components
	//NB: entityExists is used so that we can operator* without checking the value exists
	while (index < (int)size && (!m_scene.entityExists(index) || !m_scene.entityHasComponents(index, m_componentIds)))
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
