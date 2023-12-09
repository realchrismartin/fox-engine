#include "Game.hpp"

#include "src/graphics/Camera.hpp"
#include "src/systems/Systems.hpp"
#include "src/scenes/ExampleTestScene.hpp"

const float Game::TIMESTEP = .0167f;

Game::Game()
{
	//Say hi!
	Logger::log("Welcome to Chris n' J's game, you freak!");
}

void Game::play()
{
	 //For now, we create our camera on the stack
	 Camera camera = Camera();

	 //For now, we create an example scene on the stack
	 Scene scene = ExampleTestScene();

	 //TODO: add a way to change scenes here! Inform the camera when we do.

	 float currentTime = m_clock.getElapsedTime().asSeconds();
	
	 float accumulator = 0.f;

 	 //This is the main game loop.
	 while (m_window.isOpen())
	 {
		 float newTime = m_clock.getElapsedTime().asSeconds();

		 float frameTime = newTime - currentTime;

		 currentTime = newTime;

		 accumulator += frameTime;

		 while (accumulator >= TIMESTEP)
		 {
			 Systems::update(m_window, scene, camera, TIMESTEP);
			 accumulator -= TIMESTEP;
		 }
			
		 Systems::render(m_window, scene, camera);
	 }
}
