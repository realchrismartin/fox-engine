#ifndef ENTITYFILTER_HPP
#define ENTITYFILTER_HPP

#include "src/components/ComponentPool.hpp"
#include "src/scenes/Scene.hpp"
#include "src/entities/EntityIterator.hpp"

/// @brief A wrapper that creates a "view" of a Scene, i.e. access to a subset of the Entities in the scene, based on which components are specified in the variadic templating.
/// @tparam ...T 
template <typename... T> 
class EntityFilter
{
public:
	EntityFilter(Scene& scene) : m_scene(scene)
	{
		//TODO: make this use a more efficient approach later
		for (auto const& typeId : { GetComponentTypeId<T>() ...})
		{
			m_componentIds.push_back(typeId);
		}
	}

	EntityIterator begin()
	{
		return EntityIterator(m_scene, m_componentIds, 0).begin();
	};

	EntityIterator end()
	{
		return EntityIterator(m_scene, m_componentIds, 0).end();
	}

private:
	std::vector<int> m_componentIds;
	Scene& m_scene;
};
#endif
