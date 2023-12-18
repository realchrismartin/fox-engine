#include "src/Clock.hpp"

#include "SDL3/SDL.h"

float Clock::getElapsedTimeInSeconds()
{
	//At some point later we may wish to reset this when we get the value.
	return SDL_GetTicks() * 0.001f;
}