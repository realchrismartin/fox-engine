#ifndef EventTYPES_HPP
#define EventTYPES_HPP

#include "src/scenes/SceneEnum.hpp"

/// This file contains all Event types that can be sent to the relay.
/// If you add a new type, update Subscriber.cpp to add new concrete constructor and destructor definitions for the type.

struct WindowEvent 
{
	std::optional<SDL_Event> windowEvent = std::nullopt;
};

struct InputEvent
{
	std::optional<SDL_Event> inputEvent = std::nullopt;
};

#endif