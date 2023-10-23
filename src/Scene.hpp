#ifndef SCENE_HPP
#define SCENE_HPP

#include "src/entities/GameEntity.hpp"
#include "src/components/ComponentPool.hpp"

class System;

class Scene
{
public:
	std::optional<int> createEntity();
	std::vector<GameEntity>& getEntities();

	//Add a component to the entity specified by the ID
	//This involves assigning an existing component from our component pools, or allocating a new one.
	template<typename T>
	void addComponent(int entityUID)
	{
		//Handle case where we don't have an entity with this ID yet
		if (entityUID >= m_gameEntities.size())
		{
			return;
		}

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
			m_componentPools[componentTypeId] = new ComponentPool(sizeof(T), m_maxEntities);
		}

		//Use placement new operator to allocate our component at the index associated with the entity.
		//NB: we can't exceed the max m_maxEntities because there's not enough room in each component pool to support those entities!
		T* unused = new (m_componentPools[componentTypeId]->get(entityUID)) T();

		//Tell the entity it has a component of this type now
		m_gameEntities[entityUID].registerComponent(componentTypeId);
	};

	//TODO: later bushwhack these asserts
	template<typename T>
	T& getComponent(const int entityUID)
	{
		//Handle case where we don't have an entity yet
		if (entityUID >= m_gameEntities.size())
		{
			assert(false);
		}

		//Grab the id for the type of component we are getting.
		int componentTypeId = GetComponentTypeId<T>();

		//TODO: inefficiencies ho!
		std::vector<int> components = { componentTypeId };

		//Handle case where we don't have a registered component of this type for this entity
		if (!m_gameEntities[entityUID].hasAllComponents(components))
		{
			assert(false);
		}

		//Handle case that there is no component pool for this component type, which is definitely an error. Just choke on it.
		if (m_componentPools.size() < (componentTypeId + 1))
		{
			assert(false);
		}

		//If we have a component in the specified pool for this entity, return a pointer to it.
		T* pointer =  static_cast<T*>(m_componentPools[componentTypeId]->get(entityUID));

		return *pointer;
	}

	template<typename T>
	T& getComponent(const GameEntity& entity)
	{
		return getComponent<T>(entity.getUID());
	}

private:
	int m_maxEntities = 10;
	std::vector<GameEntity> m_gameEntities;
	std::vector<ComponentPool*> m_componentPools;
};

#endif