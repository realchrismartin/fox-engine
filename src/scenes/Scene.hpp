#ifndef SCENE_HPP
#define SCENE_HPP

#include "src/entities/GameEntity.hpp"
#include "src/components/ComponentPool.hpp"

class System;

/// @brief An association of Entities with their Components.
/// @brief Represents the game world and holds all entities and their components that are in the world.
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
		if (!m_componentTypeToPoolMap.count(componentTypeId))
		{
			assert(false);
		}

		//Handle case where we don't have a registered component of this type for this entity
		//NB: sometimes we may have memory allocated in the right slot due to a prior entity owning a component in this slot. we dont want to return that
		if (!m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->hasRegisteredEntity(entityUID))
		{
			assert(false);
		}

		void* componentData = m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->getComponent(entityUID);

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
		
		//Grab the id for the type of component we are adding
		int componentTypeId = GetComponentTypeId<T>();

		if (!m_componentTypeToPoolMap.count(componentTypeId))
		{
			//The pool for this component type isn't set up yet. Gotta set it up!
			m_componentPools.push_back(std::move(std::make_unique<ComponentPool>(sizeof(T), m_maxEntities)));

			//Assign a pool index for this component ID.
			m_componentTypeToPoolMap[componentTypeId] = (int)m_componentPools.size() - 1;
		}

		//Tell the pool that the entity with the given UID is using a given component memory chunk
		m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->registerEntity<T>(entityUID);
	};

private:
	size_t m_maxEntities = 10; //The max number of entities we can have, mostly dictated by the size of the component pools for now
	std::unordered_map<int, int> m_gameEntityMap; //Map of entity UIDs to the entity placement in the entity vector
	std::unordered_map<int, int> m_componentTypeToPoolMap; //Map of component types to the pool placement in the pool vector
	std::vector<GameEntity> m_gameEntities; //Worth noting: entity destructors get called a lot because we store them directly in the vector here.
	std::vector<std::unique_ptr<ComponentPool>> m_componentPools;
	int m_availableEntityUID = 0;
};

#endif