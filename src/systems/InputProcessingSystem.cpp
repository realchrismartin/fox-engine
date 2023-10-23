#include "src/systems/InputProcessingSystem.hpp"

#include "src/Scene.hpp"
#include "src/entities/GameEntity.hpp"
#include "src/entities/EntityFilter.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"

void InputProcessingSystem::update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events)
{
	 for (auto const& entity : EntityFilter<InputComponent,TransformComponent>(scene))
	 {
		 InputComponent& input = scene.getComponent<InputComponent>(entity.getUID());
		 scene.getComponent<TransformComponent>(entity.getUID()).updateWithInput(input);
	 }
}

