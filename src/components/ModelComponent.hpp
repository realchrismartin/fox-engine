#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

struct ModelConfig;
struct MeshConfig;

#include "src/graphics/Vertex.hpp"

/// @brief A component that contains data required to represent and render a 3D model in the scene.
/// @brief A model consists of 0 ... n Meshes which are made of vertices and indices. One mesh is drawn at a time to represent an "animation". The Scene controls which mesh is drawn by setting the active mesh index.
class ModelComponent
{
public:

	/// @brief Load the model component with mesh data. This sets up its vertices and indices.
	/// @param modelData 
	void loadModel(const ModelConfig& modelData);

	/// @brief Get the number of meshes loaded into this model
	/// @return 
	size_t getNumMeshes() const;

	/// @brief Get the index of the mesh that will be drawn to the screen, i.e. the "current animation frmae"
	/// @return 
	size_t getActiveMeshIndex() const;

	/// @brief Set the mesh index that will be drawn to the screen, i.e. the "current animation frame"
	/// @param meshIndex 
	void setActiveMesh(size_t meshIndex);

	/// @brief Get the number of vertices in the active mesh
	/// @return 
	size_t getVertexCount() const;

	/// @brief Get the number of indices in the active mesh
	/// @return 
	size_t getIndexCount() const;

	/// @brief Get the vertices in the active mesh
	/// @return 
	const std::vector<Vertex>& getVertices() const;

	/// @brief Get the indices in the active mesh
	/// @return 
	const std::vector<GLuint>& getIndices() const;

	/// @brief Set which MVP transform this model is using. Handled by the Scene, don't touch this!
	/// @param transformPoolIndex 
	void setTransformPoolIndex(size_t transformPoolIndex);
private:
	void loadKeyframe(size_t currentKeyframe, size_t keyframeCount, size_t framesPerMesh, const MeshConfig& meshData, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor);
	void loadFace(size_t meshIndex, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& textureCoordinates, const std::vector<glm::vec3>& vertexNormals, const std::vector<std::string>& faceData, std::map<std::string, GLuint>& faceMap);

	size_t m_activeMeshIndex = 0;

	std::vector<std::vector<GLuint>> m_meshIndices;
	std::vector<std::vector<Vertex>> m_meshVertices;
};

#endif