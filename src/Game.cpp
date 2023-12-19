#include "Game.hpp"

#include "SDL3/SDL.h"

#include "src/graphics/Camera.hpp"
#include "src/systems/Systems.hpp"
#include "src/util/Logger.hpp"
#include "src/scenes/SceneLibrary.hpp"

const float Game::TIMESTEP = .0167f;

bool Game::initSDL()
{
	return true;
}

void Game::play()
{
	//Try to initialize our windowing library.
	//If we can't, bomb out here.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        SDL_Log("SDL_Init_VIDEO failed (%s)", SDL_GetError());
		SDL_Quit();
		return;
    }
	
	 //Initialize the game elements on the stack now. 
	 Clock clock = Clock();
	 Window window = Window();
	 Camera camera = Camera();

	 //For now, we create an example scene on the stack
	 Scene scene = Scene(SceneEnum::TEST_SCENE);

	 float currentTime = clock.getElapsedTimeInSeconds();
	
	 float accumulator = 0.f;

 	 //This is the main game loop.
	 while (window.isOpen())
	 {
		 float newTime = clock.getElapsedTimeInSeconds();

		 float frameTime = newTime - currentTime;

		 currentTime = newTime;

		 accumulator += frameTime;

		 while (accumulator >= TIMESTEP)
		 {
			 Systems::update(window, scene, camera, TIMESTEP);
			 accumulator -= TIMESTEP;
		 }
			
		 Systems::render(window, scene, camera);
	 }

	 Logger::log("See you next time, space fox boy...");
}
