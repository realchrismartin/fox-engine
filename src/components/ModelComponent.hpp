#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

struct ModelData
{
	glm::vec2 textureSize = glm::vec2(2000, 2000);
	glm::vec2 spriteSize = glm::vec2(509, 509);
	glm::vec2 spriteOffsetOnTexture = glm::vec2(0, 0);
	std::string modelFilePath = "../../img/windmill.obj";
};

class ModelComponent
{
public:
	ModelComponent();
	void loadModel(const ModelData& model);
	const std::vector<GLfloat>& getVertices() const;
	const std::vector<GLuint>& getIndices() const;
private:
	void loadFace(const std::vector<glm::vec3>& vertices, 
		const std::vector<glm::vec2>& textureCoordinates, 
		const std::vector<glm::vec3>& vertexNormals, 
		const std::vector<std::string>& faceData, 
		std::map<std::string, GLuint>& faceMap, 
		const glm::vec2& textureCoordinateRatio,
		const glm::vec2& spriteOffsetOnTexture
		);

	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;

	ModelData m_modelData;
};

#endif