#ifndef COMPONENTTYPES_HPP
#define COMPONENTTYPES_HPP

#include <typeindex>

//TODO: protect this better later
static std::unordered_map<std::type_index, int> s_componentTypeIdMap;
static int s_componentTypeId = 0;

/// @brief A static method that modifies global state in order to allow us to create unique ids for component types at runtime.
/// @brief This allows us to make component pools which have IDs, and also use these IDs elsewhere too!
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

#endif
