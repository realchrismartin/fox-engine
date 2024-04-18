#ifndef TEXTCONFIG_HPP
#define TEXTCONFIG_HPP

struct TextConfig
{
	std::string textToDisplay = "J is sleepy";
	unsigned int fontSize = 5;
	size_t charactersPerLine = 20;
	bool centered = false;
	bool animated = false;
	glm::vec2 margin = glm::vec2(0.f, 0.f);
	glm::vec2 textureSize = glm::vec2(2500.f,2500.f); //TODO: move this and make it integers later

	//TODO: later, associate this with a specific font, which maps to a texture.
};

#endif