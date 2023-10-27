#include "src/scenes/Scene.hpp"
#include "src/util/Logger.hpp"

std::optional<int> Scene::createEntity()
{
	if (m_gameEntities.size() + 1 > m_maxEntities) //TODO: is the +1 off by one? ;3
	{
		Logger::log("There are too many entities, please increase allowable entities via m_maxEntities or decrease number of present entities via m_gameEntities.");
		return std::nullopt;
	}

	int uid = getNextAvailableUID();
	m_gameEntities.push_back(GameEntity(uid));
	m_gameEntityMap[uid] = (int)m_gameEntities.size() - 1; //Map the UID to the slot we're using for the entity

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

	//Problem: We gotta ensure the Game Entities vector is tightly packed so we can iterate over it without having holes in it, but we still wanna remove stuff from it.
	//TLDR solution: if we remove an entity, swap the last entity in the vector with the entity we're removing.

	//For each component type the erased entity has, we don't bother resetting the component data. It is just "still there" in the pools.
	//If we addComponent to the entity later, addComponent will overwrite whatever is at this entity index at that point using placement new.
	//Since EntityFilter only returns entities that say they have specific components, we will never use the "dead" components that are orphaned this way unless we have logically overwritten them first.

	int slotBeingReplaced = m_gameEntityMap.at(uid);
	int lastSlot = (int)m_gameEntities.size() - 1;

	if (slotBeingReplaced == lastSlot)
	{
		//This is the trivial case - removing the last entity. We don't need to swap it, just remove it straight up
		m_gameEntities.pop_back();
		m_gameEntityMap.erase(uid);
		return;
	}

	//If this isn't the trivial case:
	//We are gonna swap whatever the last entity is in game entities into the slot that's being emptied, then we're gonna shorten the game entities vector.

	//Swap all of the back slot game entity's components into this entity's component slots
	for (auto const& componentTypeUID : m_gameEntities.at(lastSlot).getComponentTypes())
	{
		//Swap the component of this type into the "slotBeingReplaced", overwriting whatever is there.
		m_componentPools[componentTypeUID]->copyComponent(lastSlot, slotBeingReplaced);
	}

	//Update the mapping to point the newly-moved entity to the slot being replaced.
	m_gameEntityMap[m_gameEntities.at(lastSlot).getUID()] = slotBeingReplaced;
	m_gameEntities[slotBeingReplaced] = m_gameEntities.at(lastSlot); // Copy the last slot entity into the replacing slot

	m_gameEntities.pop_back(); //Remove the last entity now that its been copied to the freed slot
	m_gameEntityMap.erase(uid); //Remove the original entity from the map
}

std::vector<GameEntity>& Scene::getEntities()
{
	return m_gameEntities;
}

int Scene::getNextAvailableUID() const
{
	int uid = 0;

	for (auto const& [key, value] : m_gameEntityMap)
	{
		if (key >= uid)
		{
			uid = key + 1;
		}
	}

	return uid;
}
