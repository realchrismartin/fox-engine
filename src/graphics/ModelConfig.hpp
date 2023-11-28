#ifndef MODELDATA_HPP
#define MODELDATA_HPP

/// @brief Defines a configuration for one mesh in a model
struct MeshConfig
{
	std::string meshFilePath = "../../img/windmill.obj"; //File path to the OBJ file that contains vertices, indices, and texture data.
};

/// @brief Defines a configuration for a 3d model that contains 0 ... n meshes and has a sprite that it maps to that mesh.
/// @brief All of the meshes are assumed to use the same texture.
struct ModelConfig
{
	//TODO: allow for specification of alternate texture atlases?
	glm::vec2 textureSize = glm::vec2(2500, 2500); //How big the texture atlas is, in pixels.
	glm::vec2 spriteSize = glm::vec2(509, 509); //How big the sprite is in the texture atlas, in pixels
	glm::vec2 spriteOffsetOnTexture = glm::vec2(0, 0); //Where the sprite is in the texture atlas. //TODO: specify units this is in.

	std::vector<MeshConfig> meshes; //Contains all of the mesh data associated with this model
};

#endif