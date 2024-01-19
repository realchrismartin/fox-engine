#ifndef MESSAGETYPES_HPP
#define MESSAGETYPES_HPP

#include "src/scenes/SceneEnum.hpp"

/// This file contains all message types that can be sent to the relay.
/// If you add a new type, update Recipient.cpp to add new concrete constructor and destructor definitions for the type.

/// @brief Scenes listen for this message type to see if the scene should be changed.
struct SceneChangeMessage
{
	SceneEnum requestedScene = SceneEnum::MAIN_MENU;
};

#endif