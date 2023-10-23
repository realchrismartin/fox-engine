#include "src/systems/RenderSystem.hpp"

#include "src/Scene.hpp"
#include "src/components/GraphicsComponent.hpp"
#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

void RenderSystem::update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events)
{
	 for (auto const& entity : EntityFilter<GraphicsComponent>(scene))
	 {
		 scene.getComponent<GraphicsComponent>(entity.getUID()).draw(window);
	 }
}

