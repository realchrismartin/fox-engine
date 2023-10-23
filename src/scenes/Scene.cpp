#include "src/scenes/Scene.hpp"
#include "src/util/Logger.hpp"

std::optional<int> Scene::createEntity()
{
	//TODO: using the size of the entity list will be problematic if we want to remove entities later.
	int uid = (int)m_gameEntities.size();
	
	if (uid > m_maxEntities) 
	{
		Logger::log("There are too many entities, please increase allowable entities via m_maxEntities or decrease number of present entities via m_gameEntities.");
		return std::nullopt;
	}

	GameEntity entity(uid);
	m_gameEntities.push_back(entity);

	return uid;
}

std::vector<GameEntity>& Scene::getEntities()
{
	return m_gameEntities;
}
