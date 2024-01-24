#ifndef MESSAGETYPES_HPP
#define MESSAGETYPES_HPP

#include "src/scenes/SceneEnum.hpp"

/// This file contains all message types that can be sent to the relay.
/// If you add a new type, update Recipient.cpp to add new concrete constructor and destructor definitions for the type.

struct WindowMessage 
{
	std::optional<SDL_Event> windowEvent = std::nullopt;
};

struct InputMessage
{
	std::optional<SDL_Event> inputEvent = std::nullopt;
};

#endif