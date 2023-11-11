#include "src/scenes/Scene.hpp"
#include "src/util/Logger.hpp"


void Scene::loadModel(const ModelData& modelData, int entityUID)
{
	//TODO: find the components for this model. If they both exist, reset them and load the model data into them.

	//TODO: if entity doesn't eixst, stop

	//TODO: if entity doesn't have a model component, add one

	//TODO: if entity doesn't have a vertices component, add one

	//TODO: if entity doesn't have an indices component, add one

	//TODO: call loadData on the model component given the vertex and index components to load it up
}

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

std::optional<int> Scene::getCameraEntity() const
{
	return m_cameraEntityId;
}

std::optional<int> Scene::getCameraTargetEntity() const
{
	return m_cameraTargetEntityId;
}

void Scene::addChild(int parentEntityUID, int childEntityUID)
{
	//Condition: the parent has to exist as an entity
	if (!m_gameEntityMap.count(parentEntityUID))
	{
		//This entity was never registered.
		return;
	}

	//Condition: this entity can't already be a child (it's allowed to be a root node though)
	for (auto const& [parent, children] : m_sceneGraph)
	{
		if (children.count(childEntityUID))
		{
			return;
		}
	}

	//All good - we can add this entity as a child of this parent
	if (!m_sceneGraph.count(parentEntityUID))
	{
		m_sceneGraph.insert({ parentEntityUID, { childEntityUID } });
	}
	else 
	{
		m_sceneGraph.at(parentEntityUID).insert(childEntityUID);
	}

	//If we had the child as a root node before, remove it now - it has a parent!
	m_rootNodes.erase(childEntityUID);
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
	m_rootNodes.insert(uid); //By default, the entity has no parents, so it is a root node.

	m_availableEntityUID++;

	return uid;
}

void Scene::setCameraEntity(int uid)
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

	m_cameraEntityId = uid;
}

void Scene::setCameraTargetEntity(int uid)
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

	m_cameraTargetEntityId = uid;
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

	//If this entity is the camera, lose it
	if (m_cameraEntityId.has_value() && m_cameraEntityId.value() == uid)
	{
		m_cameraEntityId = std::nullopt;
	}

	//If this entity is the camera target, lose it
	if (m_cameraTargetEntityId.has_value() && m_cameraTargetEntityId.value() == uid)
	{
		m_cameraTargetEntityId = std::nullopt;
	}

	//Remove the entity being removed from the scene graph

	//Remove this uid as a child
	for (auto& [parent, children] : m_sceneGraph)
	{
		if (children.count(uid))
		{
			children.erase(uid);
		}
	}

	std::set<int> childrenToErase;

	//Remove this uid as a parent
	if (m_sceneGraph.count(uid))
	{
		//If we want to remove an entity and it has children, we assume we wanna remove the children too
		for (auto& child : m_sceneGraph.at(uid))
		{
			childrenToErase.insert(child);
		}

		m_sceneGraph.erase(uid);
	}

	//Remove this uid as a root node
	m_rootNodes.erase(uid);

	//Now that the graph is updated to remove this entity, erase children too.
	//TODO: CHECK THIS LOGIC <---------------------------------
	for (auto const& child : childrenToErase)
	{
		removeEntity(child);
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
