#include "Game.hpp"

#include "src/graphics/Camera.hpp"
#include "src/systems/Systems.hpp"
#include "src/scenes/ExampleTestScene.hpp"

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

 	 //This is the main game loop.
     //TODO: we need some semblance of a constant time tick per update here.
	 while (m_window.isOpen())
	 {
		 Systems::update(m_window, scene, camera, (float)m_clock.restart().asMilliseconds());
		 Systems::render(m_window, scene, camera);
	 }
}
