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
	}

	/// @brief Get the component at index. We can do this because we know how big each one is.
	inline void* get(size_t index)
	{
		return m_data + (index * m_componentSize);
	}

	~ComponentPool()
	{
		delete[] m_data;
	}

private:
	int* m_data = nullptr;
	size_t m_componentSize = 0;
};

#endif
