#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

struct ModelConfig;
struct MeshConfig;

#include "src/graphics/Vertex.hpp"

class ModelComponent
{
public:
	size_t getNumMeshes() const;
	size_t getActiveMeshIndex() const;
	void setActiveMesh(size_t meshIndex);
	size_t getVertexCount() const;
	size_t getIndexCount() const;
	const std::vector<GLuint>& getIndices() const;
	const std::vector<Vertex>& getVertices() const;
	void setTransformPoolIndex(size_t transformPoolIndex);
	void loadModel(const ModelConfig& modelData);
private:
	void loadMesh(size_t meshIndex, const MeshConfig& meshData);
	void loadFace(size_t meshIndex, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& textureCoordinates, const std::vector<glm::vec3>& vertexNormals, const std::vector<std::string>& faceData, std::map<std::string, GLuint>& faceMap, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor);

	size_t m_activeMeshIndex = 0;

	std::vector<std::vector<GLuint>> m_indices;
	std::vector<std::vector<Vertex>> m_vertices;
};

#endif