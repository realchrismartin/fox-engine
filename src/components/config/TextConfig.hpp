#ifndef TEXTCONFIG_HPP
#define TEXTCONFIG_HPP

#include "src/graphics/TextureMapper.hpp"

struct TextConfig
{
	TextureEnum texture = TextureEnum::SPRITE_SHEET_1;
	std::string textToDisplay = "J is sleepy";
	unsigned int fontSize = 5;
	size_t charactersPerLine = 20;
	bool centered = false;
	bool animated = false;
	glm::vec2 margin = glm::vec2(0.f, 0.f);
};

#endif