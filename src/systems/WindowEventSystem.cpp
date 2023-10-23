#include "src/systems/WindowEventSystem.hpp"

#include "src/Scene.hpp"
#include "src/components/InputComponent.hpp"
#include "src/entities/EntityFilter.hpp"
#include "src/components/ComponentPool.hpp"

void WindowEventSystem::update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) 
{
	for (auto const& entity : EntityFilter<InputComponent>(scene))
	{
		InputComponent& input = scene.getComponent<InputComponent>(entity.getUID());

		input.clearEvents();

		for (auto const& event : events)
		{
			input.informOfWindowEvent(event);
		}
	}
}

