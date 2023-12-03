#ifndef MODELCONFIG_HPP
#define MODELCONFIG_HPP

/// @brief Defines a configuration for a 3d model that contains 1 ... n keyframes and has a sprite that it maps to itself.
/// @brief All of the keyframes are assumed to use the same texture.
struct ModelConfig
{
	//TODO: allow for specification of alternate texture atlases?
	glm::vec2 textureSize = glm::vec2(2500, 2500); //How big the texture atlas is, in pixels.
	glm::vec2 spriteSize = glm::vec2(509, 509); //How big the sprite is in the texture atlas, in pixels
	glm::vec2 spriteOffsetOnTexture = glm::vec2(0, 0); //Where the sprite is in the texture atlas. //TODO: specify units this is in.

	size_t frameCount = 1; //How many frames are in the animation.

	std::vector<std::string> keyframeFilePaths = { "../../img/windmill.obj" };
};

#endif