#include "src/systems/GiantMainSystem.hpp"

#include "src/Scene.hpp"
#include "src/components/GraphicsComponent.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/GameEntity.hpp"
#include <iostream>

//TODO: this system is a fuckin monolith, fix it later
void GiantMainSystem::update(Scene& scene, std::shared_ptr<sf::RenderWindow> window, std::vector<sf::Event>& events)
{
	//TODO: gotta make this iterator parameterized! :) 
	 for (auto const& entity : scene)
	 {
		 InputComponent* input = scene.getComponent<InputComponent>(entity.getUID());
		 TransformComponent* transform = scene.getComponent<TransformComponent>(entity.getUID());
		 GraphicsComponent* graphics = scene.getComponent<GraphicsComponent>(entity.getUID());
		 
		 if (input != nullptr && transform != nullptr)
		 {
			 transform->updateWithInput(input);
		 }

		 if (transform != nullptr && graphics != nullptr)
		 {
			 graphics->updateWithTransform(transform);
		 }

		 if (graphics != nullptr)
		 {
			graphics->draw(window);
		 }
	 }
}

