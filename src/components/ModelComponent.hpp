#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

struct ModelData;
class VerticesComponent;
class IndicesComponent;

class ModelComponent
{
public:
	size_t getVertexCount() const;
	void loadModel(const ModelData& modelData, VerticesComponent& verticesComponent, IndicesComponent& indicesComponent);
private:
	void loadFace(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& textureCoordinates, const std::vector<glm::vec3>& vertexNormals, const std::vector<std::string>& faceData, std::map<std::string, GLuint>& faceMap, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor, VerticesComponent& verticesComponent, IndicesComponent& indicesComponent);
	size_t m_numVertices = 0;
};

#endif