#include "src/systems/ProcessWindowEventsSystem.hpp"

#include "src/Scene.hpp"
#include "src/components/InputComponent.hpp"

void ProcessWindowEventsSystem::update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events) 
{
	//TODO: gotta make this iterator parameterized! :) 
	for (auto const& entity : scene)
	{
		InputComponent* input = scene.getComponent<InputComponent>(entity.getUID());

		if (input != nullptr)
		{
			input->clearEvents();

			for (auto const& event : events)
			{
				input->informOfWindowEvent(event);
			}
		}
	}

}

