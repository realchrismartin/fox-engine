#ifndef MODELDATA_HPP
#define MODELDATA_HPP

struct MeshConfig
{
	glm::vec2 textureSize = glm::vec2(2500, 2500);
	glm::vec2 spriteSize = glm::vec2(509, 509);
	glm::vec2 spriteOffsetOnTexture = glm::vec2(0, 0);
	std::string meshFilePath = "../../img/windmill.obj";
};

struct ModelConfig
{
	std::vector<MeshConfig> meshes;
};


#endif