#include "Game.hpp"

#include "src/graphics/Camera.hpp"
#include "src/systems/Systems.hpp"
#include "src/scenes/ExampleTestScene.hpp"
#include "SDL3/SDL.h"

const float Game::TIMESTEP = .0167f;

bool Game::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        SDL_Log("SDL_Init_VIDEO failed (%s)", SDL_GetError());
		return false;
    }

	return true;
}

void Game::play()
{
	bool init = initSDL();

	if (!init)
	{
		SDL_Quit();
		return;
	}

	/*
    while (true)
    {
        int finished = 0;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                finished = 1;
                break;
            }
        }
        if (finished) 
        {
            break;
        }
    }

    SDL_Quit();
	*/


	 //For now, we create our camera on the stack
	 Camera camera = Camera();

	 //For now, we create an example scene on the stack
	 Scene scene = ExampleTestScene();

	 float currentTime = m_clock.getElapsedTime().asSeconds();
	
	 float accumulator = 0.f;

 	 //This is the main game loop.

	 //Need to be able to exit! TODO
	 while (true)
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
