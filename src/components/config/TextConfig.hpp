#ifndef TEXTCONFIG_HPP
#define TEXTCONFIG_HPP

struct TextConfig
{
	std::string textToDisplay = "J is sleepy";
	glm::vec2 frameSize = glm::vec2(1.f,1.f); //The amount of space the text is allowed to take up on screen. 1.f is the entire screen.
	float textHeightAsScreenRatio = 1.f;
	//TODO: which texture atlas is being used?
	//TODO: add line breaks and whatnot?
};

#endif