#ifndef MESSAGETYPES_HPP
#define MESSAGETYPES_HPP

#include "src/scenes/SceneEnum.hpp"

/// @brief Scenes listen for this message type to see if the scene should be changed.
struct SceneChangeMessage
{
	SceneEnum requestedScene = SceneEnum::MAIN_MENU;
};

#endif