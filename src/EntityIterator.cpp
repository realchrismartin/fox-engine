#include "src/EntityIterator.hpp"

#include "src/GameEntity.hpp"
#include "src/Scene.hpp"

EntityIterator::EntityIterator(Scene* scene, int entityIndex)
{
	m_entityIndex = entityIndex;
	m_scene = scene;
}

int EntityIterator::getEntityIndex() const
{
	return m_entityIndex;
}

GameEntity EntityIterator::operator*() const
{
	return m_scene->getEntities()[m_entityIndex];
}

bool EntityIterator::operator==(const EntityIterator& other) const
{
	return m_entityIndex == other.getEntityIndex();
}

bool EntityIterator::operator!=(const EntityIterator& other) const
{
	return m_entityIndex != other.getEntityIndex();
}

bool EntityIterator::excludeEntity(GameEntity& entity) const
{
	return false; //TODO: we want to exclude entities that don't have certain components we want!
}

EntityIterator& EntityIterator::operator++()
{
	int size = (int)m_scene->getEntities().size();
	
	bool advanced = false;

	//Keep advancing if the now-current entry is invalid and there's more items in the entity list
	while (m_entityIndex < size && excludeEntity(m_scene->getEntities()[m_entityIndex]))
	{
		m_entityIndex++;
		advanced = true;
	}

	if (!advanced)
	{
		m_entityIndex++;
	}

	return *this;
}

const EntityIterator EntityIterator::begin() const
{
	int index = 0;

	int size = (int)m_scene->getEntities().size();

	//Skip all of the entities that don't match at the beginning
	while (index < (int)size && excludeEntity(m_scene->getEntities()[index]))
	{
		index++;
	}

	return EntityIterator(m_scene, index);
}

const EntityIterator EntityIterator::end() const
{
	//Return an iterator to the end, regardless
	return EntityIterator(m_scene, (int)m_scene->getEntities().size());
}
