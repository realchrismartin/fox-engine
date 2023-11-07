#include "src/scenes/Scene.hpp"
#include "src/util/Logger.hpp"

GameEntity& Scene::getEntity(int entityIndex)
{
	if (m_gameEntities.size() <= (size_t)entityIndex)
	{
		throw std::out_of_range("Asked for an entity that is out of range!");
	}
	
	return m_gameEntities[entityIndex];
}

int Scene::getEntityCount() const
{
	return (int)m_gameEntityMap.size();
}

std::optional<int> Scene::createEntity()
{
	if (m_gameEntities.size() >= m_maxEntities)
	{
		Logger::log("There are too many entities, please increase allowable entities via m_maxEntities or decrease number of present entities via m_gameEntities.");
		return std::nullopt;
	}
	int uid = m_availableEntityUID;

	m_gameEntities.emplace_back(uid);
	m_gameEntityMap[uid] = (int)m_gameEntities.size() - 1; //Map the UID to the slot we're using for the entity

	m_availableEntityUID++;

	return uid;
}

void Scene::removeEntity(int uid)
{
	if (!m_gameEntityMap.count(uid))
	{
		//This entity was never registered.
		return;
	}

	if (m_gameEntities.empty())
	{
		//Somehow we have no entities but have a mapped one. IDK. Its a bug.
		return;
	}

	int slotBeingReplaced = m_gameEntityMap.at(uid);
	int lastSlot = (int)m_gameEntities.size() - 1;

	int currentSlotUID = m_gameEntities.at(slotBeingReplaced).getUID();
	int lastSlotUID = m_gameEntities.at(lastSlot).getUID();

	//Problem: We gotta ensure the Game Entities vector is tightly packed so we can iterate over it without having holes in it, but we still wanna remove stuff from it.
	//TLDR solution: if we remove an entity, swap the last entity in the vector with the entity we're removing, then tell all the component pools to do that same swap.

	//For each component type the erased entity has, we don't bother resetting the component data. It is just "still there" in the pools unless erasing would require moving data over top of it.
	//If we addComponent to the entity later, addComponent will overwrite whatever is at this entity index at that point using placement new.
	//Since EntityFilter only returns entities that say they have specific components, we will never use the "dead" components that are orphaned this way unless we have logically overwritten them first.

	//Tell all the pools that this entity is using that it can use those pool elements for something else. Do this regardless of registration on the entity.
	//This doesn't clear memory or anything. What it DOES do is ensure the pool's memory is still contiguous, which may require copying component data around.

	for (auto& pool : m_componentPools)
	{
		pool->deregisterEntity(currentSlotUID);
	}

	if (slotBeingReplaced == lastSlot)
	{
		//This is the trivial case - removing the last entity. We don't need to swap it, just remove it straight up
		m_gameEntities.pop_back();
		m_gameEntityMap.erase(uid);
		return;
	}

	//If this isn't the trivial case:
	//We are gonna swap whatever the last entity is in game entities into the slot that's being emptied.

	//Update the mapping to point the newly-moved entity to the slot being replaced.
	m_gameEntityMap[m_gameEntities.at(lastSlot).getUID()] = slotBeingReplaced;
	m_gameEntities[slotBeingReplaced] = m_gameEntities.at(lastSlot); // Copy the last slot entity into the replacing slot

	m_gameEntities.pop_back(); //Remove the last entity now that its been copied to the freed slot
	m_gameEntityMap.erase(uid); //Remove the original entity from the map
}

bool Scene::entityHasComponents(int entityIndex, std::vector<int>& componentTypeIds) const
{
	if (componentTypeIds.empty())
	{
		return false; //Illogical, but failsafe condition
	}

	if (entityIndex < 0 || entityIndex >= m_gameEntities.size())
	{
		return false; //Entity index out of bounds failsafe
	}

	int uid = m_gameEntities[entityIndex].getUID();

	for (auto const& id : componentTypeIds)
	{

		if (!m_componentTypeToPoolMap.count(id))
		{
			return false; //Something is wrong. There is no pool for this type.
		}
		
		auto const& pool = m_componentPools[m_componentTypeToPoolMap.at(id)];

		//If any specified component pool hasn't registered this entity, return false
		if (!pool->hasRegisteredEntity(uid))
		{
			return false;
		}
	}

	return true;
}
