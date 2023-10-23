#include "src/systems/GraphicsTransformSystem.hpp"

#include "src/Scene.hpp"
#include "src/components/GraphicsComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

void GraphicsTransformSystem::update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events)
{
	 for (auto const& entity : EntityFilter<GraphicsComponent>(scene))
	 {
		 TransformComponent& transform = scene.getComponent<TransformComponent>(entity.getUID());
		 scene.getComponent<GraphicsComponent>(entity.getUID()).updateWithTransform(transform);
	 }
}

