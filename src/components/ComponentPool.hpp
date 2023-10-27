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

	inline void registerEntity(int entityUID)
	{
		if (hasRegisteredEntity(entityUID))
		{
			return; //Already registered
		}

		if (m_freeSlots.empty() && m_componentsUsedCount >= m_capacity)
		{
			return; //We're at capacity, can't hold any more components. Dont register
		}

		//Save a mapping indicating that the entity with this UID will be using the component at the specified index.

		//Use a free slot if we can
		if (!m_freeSlots.empty())
		{
			m_entityToComponentMap[entityUID] = m_freeSlots.back();
			m_freeSlots.pop_back();
		}
		else 
		{
			//Use a new slot
			//Once we use up all the slots, we can only use freed slots.
			m_entityToComponentMap[entityUID] = m_componentsUsedCount;
			m_componentsUsedCount++;
		}
	}

	inline bool hasRegisteredEntity(int entityUID)
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

		//Free up a slot here.
		m_freeSlots.push_back(m_entityToComponentMap.at(entityUID));

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
	std::vector<int> m_freeSlots;
};

#endif
