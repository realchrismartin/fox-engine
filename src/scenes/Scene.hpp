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

	/// @brief Get the component of the specified type T that is associated with the entity with the UID entityUID
	/// @tparam T The type of component we are asking for
	/// @param entityUID The entity we want to get a component for
	/// @return A reference to the component of the type specified.
	template<typename T>
	T& getComponent(const int entityUID)
	{
		//Handle case where we don't have an entity yet
		if (!m_gameEntityMap.count(entityUID))
		{
			throw std::out_of_range("There is no entity with this UID.");
		}
		
		//Grab the id for the type of component we are getting.
		int componentTypeId = GetComponentTypeId<T>();

		//Handle case that there is no component pool for this component type, which is definitely an error. Just choke on it.
		if (!m_componentTypeToPoolMap.count(componentTypeId))
		{
			throw std::out_of_range("There is no component pool for this component type");
		}

		//Handle case where we don't have a registered component of this type for this entity
		if (!m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->hasRegisteredEntity(entityUID))
		{
			throw std::out_of_range("There is no registered component of this type for this entity");
		}

		//Get the component data that is associated with this entity UID
		void* componentData = m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->getComponent(entityUID);

		//If the pool can't find a component for this entity (unlikely, because we checked that it was registered first), it may return nullptr for the data.
		//If this happens, choke on it.
		if (componentData == nullptr)
		{
			throw std::runtime_error("Getting the component for this entity from the pool returned nullptr");
		}

		//If we have a component in the specified pool for this entity, return a pointer to it.
		T* pointer = static_cast<T*>(componentData);

		//TODO: please move this later this is very expensive to do here....
		if (!pointer->getChildren().empty())
		{
			for (auto const& child : pointer->getChildren())
			{
				if(m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->hasRemovedEntity(child))
				{
					pointer->removeChild(child);
				}
			}
		}

		return *pointer;
	}

	/// @brief Get the component of type T associated with this GameEntity.
	/// @tparam T The component type we want to get
	/// @param entity The entity we are getting a component for
	/// @return 
	template<typename T>
	T& getComponent(const GameEntity& entity)
	{
		return getComponent<T>(entity.getUID());
	}

	/// @brief Return true if the entity at this index in the entity list has registered components with the specified IDs. 
	/// @param entityIndex  The index of the entity. This is not the entity UID.
	/// @param componentTypeIds 
	/// @return 
	bool entityHasComponents(int entityIndex, std::vector<int>& componentTypeIds) const;

	GameEntity& getEntity(int entityIndex);
	int getEntityCount() const;

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
			m_componentPools.push_back(std::move(std::make_unique<ComponentPool>(componentTypeId,sizeof(T), m_maxEntities)));

			//Assign a pool index for this component pool we just added.
			m_componentTypeToPoolMap[componentTypeId] = (int)m_componentPools.size() - 1;
		}

		//Tell the pool that the entity with the given UID is using a given component memory chunk now.
		m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->registerEntity<T>(entityUID);
	};

private:
	size_t m_maxEntities = 10; //The max number of entities we can have, mostly dictated by the size of the component pools for now
	std::map<int, int> m_gameEntityMap; //Map of entity UIDs to the entity placement in the entity vector
	std::unordered_map<int, int> m_componentTypeToPoolMap; //Map of component types to the pool placement in the pool vector
	std::vector<GameEntity> m_gameEntities; //Worth noting: entity destructors get called a lot because we store them directly in the vector here.
	std::vector<std::unique_ptr<ComponentPool>> m_componentPools;
	int m_availableEntityUID = 0;
};

#endif