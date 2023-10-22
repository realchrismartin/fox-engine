#include "src/Scene.hpp"

#include "src/systems/GiantMainSystem.hpp"
#include "src/systems/ProcessWindowEventsSystem.hpp"
#include "src/EntityIterator.hpp"
#include "src/Logger.hpp"

Scene::Scene()
{
	//TODO: right now we just manually add the systems we care about when we create a scene. They are pretty coupled.
	m_systems.push_back(std::make_shared<GiantMainSystem>());
	m_systems.push_back(std::make_shared<ProcessWindowEventsSystem>());
};

std::optional<int> Scene::createEntity()
{
	//TODO: this will be problematic if we want to remove entities later.
	int uid = (int)m_gameEntities.size();
	if (uid > m_maxEntities) {
		Logger::log("There are too many entities, please increase allowable entities via m_maxEntities or decrease number of present entitites via m_gameEntities.");
		return std::nullopt;
	}
		

	GameEntity entity(uid);
	m_gameEntities.push_back(entity);
	return uid;

}

void Scene::update(std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events)
{
	for (auto const& system : m_systems)
	{
		system->update(*this, window, events);
	}
}

EntityIterator Scene::begin()
{
	return EntityIterator(this, 0).begin();
};

EntityIterator Scene::end()
{
	return EntityIterator(this, 0).end();
}


std::vector<GameEntity>& Scene::getEntities()
{
	return m_gameEntities;
}
