#ifndef MODELDATA_HPP
#define MODELDATA_HPP

struct ModelConfig
{
	glm::vec2 textureSize = glm::vec2(2000, 2000);
	glm::vec2 spriteSize = glm::vec2(509, 509);
	glm::vec2 spriteOffsetOnTexture = glm::vec2(0, 0);
	std::string modelFilePath = "../../img/windmill.obj";
};

#endif