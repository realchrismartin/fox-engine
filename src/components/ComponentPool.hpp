#ifndef COMPONENTPOOL_HPP
#define COMPONENTPOOL_HPP

#include <typeindex>

//TODO: protect this better later
static std::unordered_map<std::type_index, int> s_componentTypeIdMap;
static int s_componentTypeId = 0;

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

	inline void copyComponent(size_t fromIndex, size_t toIndex)
	{
		int* src = m_data + (fromIndex * m_componentSize);
		int* dest = m_data + (toIndex * m_componentSize);

		std::memcpy(dest, src, m_componentSize);
	}

	inline void deregisterEntity(int entityUID)
	{
		//NB: this does NOT clean up pool memory or anything. If we register an entity again, we assume we are going to either overwrite the memory immediately OR use whatever's there arleady.

		if (!m_entityToComponentMap.count(entityUID))
		{
			return; //Slot empty
		}

		//Free up a slot here.
		m_freeSlots.push_back(m_entityToComponentMap.at(entityUID));

		m_entityToComponentMap.erase(entityUID);
	}

	inline void registerEntity(int entityUID)
	{
		if (m_entityToComponentMap.count(entityUID))
		{
			return; //This entity is already registered
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
			m_entityToComponentMap[entityUID] = m_componentsUsedCount;
			m_componentsUsedCount++;
		}
	}

	/// @brief Get the component at the specified entity uid's index.
	///. We can do this because we know how big each one is.
	inline void* getComponent(int entityUID)
	{
		if(!m_entityToComponentMap.count(entityUID))
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
