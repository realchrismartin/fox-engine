#ifndef TEXTCONFIG_HPP
#define TEXTCONFIG_HPP

struct TextConfig
{
	std::string textToDisplay = "J is sleepy";
	unsigned int fontSize = 1;
	size_t charactersPerLine = 10;
	size_t maxLines = 5;
	//TODO: which texture atlas is being used?
};

#endif