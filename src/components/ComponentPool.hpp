#ifndef COMPONENTPOOL_HPP
#define COMPONENTPOOL_HPP

#include "src/components/ComponentTypes.hpp"
#include <ranges>

/// @brief A chunk of memory that contains enough space to store components.
/// @brief It also handles knowing which GameEntities "own" the components in that memory pool.
/// @brief A Scene owns a number of these (one per Component type), and handles registering/deregistering entities with each pool.
/// @brief Each pool has a fixed size determined by the Scene. Right now this equals the maximum number of entities, because we assume we might want one of each component for each entity.
//TODO: That's a bit wasteful now isn't it?
class ComponentPool
{
public:

	ComponentPool(int componentTypeId, size_t componentSize, size_t capacity)
	{
		m_componentTypeId = componentTypeId;

		//Store info on how big the component is that we're storing in this pool
		m_componentSize = componentSize;

		//Allocate enough memory to store n components of size elementSize, where n = capacity
		m_data = new char[componentSize * capacity];

		m_capacity = capacity;
	}

	size_t getComponentsInUse() const
	{
		return m_componentsUsedCount;
	}

	template <typename T>
	void registerEntity(int entityUID)
	{
		if (hasRegisteredEntity(entityUID))
		{
			return; //Already registered
		}

		if (m_componentsUsedCount >= m_capacity)
		{
			return; //We're at capacity, can't hold any more components. Dont register
		}

		//Save a mapping indicating that the entity with this UID will be using the component at the specified index.
		//We always register new components at the end of the usable list, so the memory will be at the end of the list too 
		m_entityToComponentMap[entityUID] = (int)m_componentsUsedCount;
		m_componentToEntityMap[(int)m_componentsUsedCount] = entityUID;

		//Increment the component count - the next registered component will use this index.
		m_componentsUsedCount++;

		//Assign the memory using placement new at the specified location (where the component pool has a spot registered for the entity)
		//NB: if we already had a component in this slot (i.e. because we erased an entity that had the component), this should overwrite that component now.
		T* unused = new (getComponent(entityUID)) T();
	}

	bool hasRegisteredEntity(int entityUID) const
	{
		return m_entityToComponentMap.count(entityUID);
	}

	void deregisterEntity(int entityUID)
	{
		//NB: this does NOT clean up pool memory or anything. If we register an entity again, we assume we are going to either overwrite the memory immediately OR use whatever's there arleady.

		if (!hasRegisteredEntity(entityUID))
		{
			return; //This entity can't be deregistered since it's not registered
		}

		//This is where the component is now.
		int componentIndex = m_entityToComponentMap[entityUID];
		int lastComponentIndex = (int)m_componentsUsedCount - 1;

		//Trivial case - the component for the entity we're removing is at the end of the pool
		if (componentIndex == lastComponentIndex)
		{
			//Reduce the component count - we just ignore that memory. We will overwrite it in registerEntity later, maybe, but it wont be used.
			m_componentsUsedCount--;
			m_entityToComponentMap.erase(entityUID);
			m_componentToEntityMap.erase(lastComponentIndex);
			return;
		}

		//Less-trivial case - the component for the entity we're removing is somewhere in the middle of the pool
		//We gotta swap the last element of the component list INTO the free spot.
		//We do this by copying the component data to the free spot.

		if (!m_componentToEntityMap.count(lastComponentIndex))
		{
			//Something is wrong. There should be an entity using this index.
			assert(false);
		}

		int lastEntityUID = m_componentToEntityMap.at(lastComponentIndex);
		
		//Copy the last entity's component to the free spot, overwriting the one for the entity we're removing
		std::memcpy(getComponent(entityUID), getComponent(lastEntityUID), m_componentSize);

		//Update both maps to indicate that we moved the entity at the end of the list to the free spot
		m_entityToComponentMap[lastEntityUID] = componentIndex;
		m_componentToEntityMap[componentIndex] = lastEntityUID;

		//Now that we have swapped, we can remove the entity we are trying to remove.
		m_componentsUsedCount--;
		m_entityToComponentMap.erase(entityUID);
		m_componentToEntityMap.erase(lastComponentIndex); //TODO: is this right?
	}

	std::optional<size_t> getIndexOfRegisteredEntity(int entityUID)
	{
		if(!hasRegisteredEntity(entityUID))
		{
			return std::nullopt; //not registered
		}

		return (size_t)m_entityToComponentMap[entityUID];

	}

	/// @brief Get the component at the specified entity uid's index.
	///. We can do this because we know how big each one is.
	void* getComponent(int entityUID)
	{
		if(!hasRegisteredEntity(entityUID))
		{
			return nullptr; //There is no component of this type registered for this entity.
		}

		return getComponentAtIndex((size_t)m_entityToComponentMap[entityUID]);
	}

	/// @brief Get the component at the actual index specified
	/// @param index 
	/// @return 
	void* getComponentAtIndex(size_t index)
	{
		if (index >= m_componentsUsedCount)
		{
			return nullptr; //There's no way this component exists
		}

		return m_data + (index * m_componentSize);
	}

	/// @brief Get the entity IDs that have registered components in this pool. This is guaranteed to be in the order the components are stored.
	/// @return 
	std::ranges::elements_view < std::ranges::ref_view<std::map<int, int>>, 1Ui64> getRegisteredEntityUIDs()
	{
		return std::views::values(m_componentToEntityMap);
	}

	void* getData() const
	{
		return m_data;
	}

	~ComponentPool()
	{
		delete[] m_data;
	}

private:
	int m_componentTypeId = 0; //Which component type this is a pool for.
	char* m_data = nullptr; //The component pool of data. It's char type because chars are 1 byte (usually), so it can fit ANYTHING.
	size_t m_componentSize = 0; //How big a single component is in bytes in the component pool
	size_t m_capacity = 0; //How many components we can hold.
	size_t m_componentsUsedCount = 0; //How many components we ARE holding currently. This can't exceed the capacity.
	std::unordered_map<int, int> m_entityToComponentMap; //Map of entity UID to component index. Used to keep that m_data array tightly packed!
	std::map<int, int> m_componentToEntityMap;
};

#endif
