#ifndef SCENE_HPP
#define SCENE_HPP

#include "src/entities/GameEntity.hpp"
#include "src/components/ComponentPool.hpp"

class System;

class Scene
{
public:
	std::vector<GameEntity>& getEntities();

	//TODO: later bushwhack these asserts
	template<typename T>
	T& getComponent(const int entityUID)
	{
		//Handle case where we don't have an entity yet
		if (!m_gameEntityMap.count(entityUID))
		{
			assert(false); //There is no entity with this ID.
		}
		
		//Grab the id for the type of component we are getting.
		int componentTypeId = GetComponentTypeId<T>();

		//Handle case that there is no component pool for this component type, which is definitely an error. Just choke on it.
		if (m_componentPools.size() < (componentTypeId + 1))
		{
			assert(false);
		}

		//Handle case where we don't have a registered component of this type for this entity
		//NB: sometimes we may have memory allocated in the right slot due to a prior entity owning a component in this slot. we dont want to return that
		if (!m_componentPools[componentTypeId]->hasRegisteredEntity(entityUID))
		{
			assert(false);
		}

		void* componentData = m_componentPools[componentTypeId]->getComponent(entityUID);

		if (componentData == nullptr)
		{
			assert(false);
		}

		//If we have a component in the specified pool for this entity, return a pointer to it.
		T* pointer = static_cast<T*>(componentData);

		return *pointer;
	}

	template<typename T>
	T& getComponent(const GameEntity& entity)
	{
		return getComponent<T>(entity.getUID());
	}

	bool entityHasComponents(int entityIndex, std::vector<int>& componentTypeIds) const;


protected:
	std::optional<int> createEntity();
	void removeEntity(int uid);

	//Add a component to the entity specified by the ID
	//This involves assigning an existing component from our component pools, or allocating a new one.
	template<typename T>
	void addComponent(int entityUID)
	{
		//Handle case where we don't have an entity with this ID yet
		if (!m_gameEntityMap.count(entityUID))
		{
			Logger::log("There is no entity with this UID. Skipping adding a component to it.");
			return; //Return having done nothing
		}
		
		int entityIndex = m_gameEntityMap.at(entityUID);

		//Grab the id for the type of component we are adding
		int componentTypeId = GetComponentTypeId<T>();

		//Resize the list of pools if we have to
		if (m_componentPools.size() <= componentTypeId)
		{
			m_componentPools.resize(componentTypeId + 1, nullptr);
		}

		//Assign a new pool if we have to
		if (m_componentPools[componentTypeId] == nullptr)
		{
			//We need to add a new pool for this component type
			//The new pool's elements need to be the size of T so that it can contain them.
			//The new pool has a capacity equivalent to the max number of entities the scene can have.
			//NB: we can't exceed the max m_maxEntities because there's not enough room in each component pool to support those entities!
			m_componentPools[componentTypeId] = new ComponentPool(sizeof(T), m_maxEntities);
			m_componentPoolCount++;
		}

		//Tell the pool that the entity with the given UID is using a given component memory chunk
		//This has to happen just prior to assigning component memory, be careful - if the memory doesn't get assigned, something might try to get it, which will assert (see getComponent)
		m_componentPools[componentTypeId]->registerEntity(entityUID);

		void* componentMemoryPointer = m_componentPools[componentTypeId]->getComponent(entityUID);

		if (componentMemoryPointer == nullptr)
		{
			return; //Somehow, there is no component in this pool registered for this entity. This ain't good.
		}

		//Assign the memory using placement new at the specified location (where the component pool has a spot registered for the entity)
		//NB: if we already had a component in this slot (i.e. because we erased an entity that had the component), this should overwrite that component now.
		T* unused = new (componentMemoryPointer) T();
	};

private:
	int m_maxEntities = 10;
	int m_componentPoolCount = 0;
	std::unordered_map<int, int> m_gameEntityMap;
	std::vector<GameEntity> m_gameEntities;
	std::vector<ComponentPool*> m_componentPools;
	int m_availableEntityUID = 0;
};

#endif