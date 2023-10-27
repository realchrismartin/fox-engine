#ifndef COMPONENTPOOL_HPP
#define COMPONENTPOOL_HPP

#include <typeindex>

//TODO: protect this better later
static std::unordered_map<std::type_index, int> s_componentTypeIdMap;
static int s_componentTypeId = 0;

/// @brief A static method that modifies global state in order to allow us to create unique ids for component types at runtime.
/// @brief This allows us to make component pools which have IDs.
/// @tparam T 
/// @return 
template <typename T>
int GetComponentTypeId()
{
	auto type = std::type_index(typeid(T));

	if (s_componentTypeIdMap.count(type))
	{
		return s_componentTypeIdMap.at(type);
	}

	s_componentTypeIdMap.emplace(type,s_componentTypeId);
	s_componentTypeId++;

	return s_componentTypeIdMap.at(type);
}

/// @brief A chunk of memory that contains enough space to store components.
/// @brief It also handles knowing which GameEntities "own" the components in that memory pool.
/// @brief A Scene owns a number of these (one per Component type), and handles registering/deregistering entities with each pool.
/// @brief Each pool has a fixed size determined by the Scene. Right now this equals the maximum number of entities, because we assume we might want one of each component for each entity.
//TODO: That's a bit wasteful now isn't it?
class ComponentPool
{
public:

	ComponentPool(size_t componentSize, size_t capacity)
	{
		//Store info on how big the component is that we're storing in this pool
		m_componentSize = componentSize;

		//Allocate enough memory to store n components of size elementSize, where n = capacity
		m_data = new int[componentSize * capacity];

		m_capacity = capacity;
	}

	template <typename T>
	inline void registerEntity(int entityUID)
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
		m_entityToComponentMap[entityUID] = m_componentsUsedCount;

		//Increment the component count - the next registered component will use this index.
		m_componentsUsedCount++;

		//Assign the memory using placement new at the specified location (where the component pool has a spot registered for the entity)
		//NB: if we already had a component in this slot (i.e. because we erased an entity that had the component), this should overwrite that component now.
		T* unused = new (getComponent(entityUID)) T();
	}

	inline bool hasRegisteredEntity(int entityUID) const
	{
		return m_entityToComponentMap.count(entityUID);
	}

	inline void deregisterEntity(int entityUID)
	{
		//NB: this does NOT clean up pool memory or anything. If we register an entity again, we assume we are going to either overwrite the memory immediately OR use whatever's there arleady.

		if (!hasRegisteredEntity(entityUID))
		{
			return; //This entity can't be deregistered since it's not registered
		}

		//This is where the component is now.
		int componentIndex = m_entityToComponentMap[entityUID];
		int lastComponentIndex = m_componentsUsedCount - 1;

		//Trivial case - the component is at the end of the in-use list
		if (componentIndex == lastComponentIndex)
		{
			//Reduce the component count - we just ignore that memory. We will overwrite it in registerEntity later, maybe, but it wont be used.
			m_componentsUsedCount--;
			m_entityToComponentMap.erase(entityUID);
			return;
		}

		//Less-trivial case - the component is somewhere in the middle of the list
		//We gotta swap the last element of the component list INTO this spot.
		//TODO: do the swap of the last element.

		bool foundComponentUID = false;
		int lastEntityUID = 0;

		//TODO: make this more efficient :(
		for (auto const& [entityUID, componentIndex] : m_entityToComponentMap)
		{
			if (componentIndex == lastComponentIndex)
			{
				//This entityUID is using the last component index.
				foundComponentUID = true;
				lastEntityUID = entityUID;
				break;
			}
		}

		if (!foundComponentUID)
		{
			//Something is wrong.
			assert(false);
		}

		//Now we have the entity UID that is using the last component index.
		//Copy that entity's component and overwrite the one at our index.
		std::memcpy(getComponent(entityUID), getComponent(lastEntityUID), m_componentSize);

		//Add that entity at our index, since it is using this index now.
		m_entityToComponentMap[lastEntityUID] = componentIndex;

		//Now that we have swapped, we can remove the entity we are trying to remove.
		m_componentsUsedCount--;
		m_entityToComponentMap.erase(entityUID);
	}

	/// @brief Get the component at the specified entity uid's index.
	///. We can do this because we know how big each one is.
	inline void* getComponent(int entityUID)
	{
		if(!hasRegisteredEntity(entityUID))
		{
			return nullptr; //There is no component of this type registered for this entity.
		}

		return m_data + ((size_t)m_entityToComponentMap[entityUID] * m_componentSize);
	}

	~ComponentPool()
	{
		delete[] m_data;
	}

private:
	int* m_data = nullptr;
	size_t m_componentSize = 0;
	size_t m_capacity = 0;
	int m_componentsUsedCount = 0;
	std::unordered_map<int, int> m_entityToComponentMap;
};

#endif
