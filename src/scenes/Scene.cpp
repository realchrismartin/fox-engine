#include "src/scenes/Scene.hpp"

#include "src/util/Logger.hpp"
#include "src/components/ModelComponent.hpp"
#include "src/components/VerticesComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/MVPTransformComponent.hpp"

void Scene::loadModel(const ModelConfig& modelData, int entityUID)
{
	if (!m_gameEntityMap.count(entityUID))
	{
		//This entity was never registered.
		return;
	}
	
	if (hasComponent<ModelComponent>(entityUID) || hasComponent<TransformComponent>(entityUID) || hasComponent<VerticesComponent>(entityUID))
	{
		//For now, don't reload model data after it's set the first time.
		Logger::log("This entity already has a model/transform/vertices/indices component. Refusing to add more.");
		return;
	}

	//Since these pools are de facto protected from being added to any other way, the indices for this entity UID match across them. This is important!
	addComponentPrivate<ModelComponent>(entityUID);
	addComponentPrivate<TransformComponent>(entityUID);
	addComponentPrivate<MVPTransformComponent>(entityUID);
	addComponentPrivate<VerticesComponent>(entityUID);

	//Do an (expensive) sanity check that all of the indices match
	ComponentPool& modelPool = getComponentPool<ModelComponent>();
	ComponentPool& transformPool = getComponentPool<TransformComponent>();
	ComponentPool& mvpTransformPool = getComponentPool<MVPTransformComponent>();
	ComponentPool& verticesPool = getComponentPool<VerticesComponent>();

	//Check to confirm that all of the indices match correctly. If they don't something is wrong
	//This should always be the last index in the pool given how pools work.
	//If it isn't just crash.
	std::optional<size_t> modelPoolIndex = modelPool.getIndexOfRegisteredEntity(entityUID);
	std::optional<size_t> transformPoolIndex = transformPool.getIndexOfRegisteredEntity(entityUID);
	std::optional<size_t> mvpTransformPoolIndex = mvpTransformPool.getIndexOfRegisteredEntity(entityUID);
	std::optional<size_t> verticesPoolIndex = verticesPool.getIndexOfRegisteredEntity(entityUID);

	if (!modelPoolIndex.has_value() || !transformPoolIndex.has_value() || !mvpTransformPoolIndex.has_value() || !verticesPoolIndex.has_value())
	{
		assert(false); //Something broke!
	}

	if (modelPoolIndex.value() != transformPoolIndex.value() || transformPoolIndex.value() != mvpTransformPoolIndex.value() || mvpTransformPoolIndex.value() != verticesPoolIndex.value() || verticesPoolIndex.value() != modelPoolIndex.value())
	{
		assert(false); //Something broke!
	}

	//Now that we have all of the components, get access to them and load them up
	ModelComponent& modelComponent = getComponent<ModelComponent>(entityUID);
	VerticesComponent& verticesComponent = getComponent<VerticesComponent>(entityUID);

	//Load the model with the local indices and the vertices component with the local vertices
	modelComponent.loadModel(modelData, verticesComponent);

	//TODO: later, this specific call could be cheaper than the one in removeEntity since it's adding to the end of the pool always.
	updateAllModelComponentAssociations();
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

void Scene::applyToSceneGraph(std::function<void(Scene&, std::optional<int>, int)>& functor)
{
	for (auto entityId : m_rootNodes)
	{
		applyFunctorToSceneGraph(std::nullopt,entityId,functor);
	}
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

	//Save whether there are model components. Use this to recalculate model component associations after updates are done.
	bool hadModelComponent = hasComponent<ModelComponent>(uid);
	bool hadTransformComponent = hasComponent<TransformComponent>(uid);
	bool hadMVPTransformComponent = hasComponent<MVPTransformComponent>(uid);
	bool hadVerticesComponent = hasComponent<VerticesComponent>(uid);

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

	//If we removed an indices component, we assume that we also removed a model/transform/vertex component
	if (hadTransformComponent && hadModelComponent && hadVerticesComponent && hadMVPTransformComponent)
	{
		updateAllModelComponentAssociations();
	}
	else if (!hadTransformComponent && !hadModelComponent && !hadVerticesComponent && !hadMVPTransformComponent)
	{
		//noop
	}
	else
	{
		assert(false); //Something is wrong. An entity has to have all of these components or none of them.
	}
}

void Scene::applyFunctorToSceneGraph(std::optional<int> parentEntityID, int entityID, std::function<void(Scene&, std::optional<int>, int)>& functor)
{
	//Run the functor on the node.
	if (parentEntityID.has_value())
	{
		functor(*this, parentEntityID.value(),entityID);
	}
	else
	{
		functor(*this, std::nullopt, entityID);
	}

	//If the node has children, call this function to apply the functors to those children
	if (m_sceneGraph.count(entityID))
	{
		for (auto child : m_sceneGraph.at(entityID))
		{
			applyFunctorToSceneGraph(entityID,child,functor);
		}
	}
}

void Scene::updateAllModelComponentAssociations()
{
	//This fn is called when any one of the four model involved pools is modified.
	//Upstream, we guaranteed that if an entity has a model, it has a transform, vertices, and indices
	//This means we can iterate over the model pool and it will be in the correct / same order as the other pools

	ComponentPool& modelPool = getComponentPool<ModelComponent>();

	size_t entityCount = 0;
	size_t vertexCount = 0;

	for (auto const& entityUID : modelPool.getRegisteredEntityUIDs())
	{
		ModelComponent& modelComponent = getComponent<ModelComponent>(entityUID);
		VerticesComponent& verticesComponent = getComponent<VerticesComponent>(entityUID);

		//Update the indices to match their position in the pool. 
		//TODO: if we reeenable the full pool upload, uncomment here
		//modelComponent.recalculateIndices(entityCount * VerticesComponent::MAX_VERTICES);
		modelComponent.recalculateIndices(vertexCount);
		vertexCount += modelComponent.getVertexCount();

		//The vertex buffer element for "mvp" is relative to the pool of transforms.
		//We have one transform per model, not per vertex.
		//We need to tell all vertices which MVP they each need to use
		verticesComponent.setTransformPoolIndex(modelComponent.getVertexCount(), entityCount);

		entityCount++;
	}
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
