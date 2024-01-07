#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

struct ModelConfig;
struct TextConfig;
struct KeyframeConfig;

#include "src/graphics/Vertex.hpp"

/// @brief Represents one part of a face of a model.
/// @brief For now we do not use normal index anywhere
struct FaceElement
{
	unsigned int vertexIndex = 0;
	unsigned int vertexTextureIndex = 0;
	unsigned int vertexNormalIndex = 0;
};

/// @brief A component that contains data required to represent and render a 3D model in the scene.
/// @brief A model consists of 0 ... n Meshes which are made of vertices and indices. One mesh is drawn at a time to represent an "animation". The Scene controls which mesh is drawn by setting the active mesh index.
class ModelComponent
{
public:

	/// @brief Load the model component given its keyframe data. This sets up its vertices and indices.
	/// @param modelData 
	void loadModel(const ModelConfig& modelConfig);

	void loadText(const TextConfig& textConfig);

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

	bool usesOrthographicProjection() const;

private:
	//Load a keyframe, generating vertices and indices for the keyframe and tween frames.
	void loadKeyframe(size_t currentKeyframe, size_t framesPerMesh, const std::string& keyframePath, std::vector<std::vector<glm::vec3>>& keyframeVertices, std::vector<std::vector<glm::vec3>>& keyframeVertexNormals, std::vector<std::vector<glm::vec2>>& keyframeTexCoords, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor);

	//Given an OBJ file path, load the data present into the provided vectors at the specified keyframe index.
	void loadOBJFile(const std::string& keyframePath, size_t currentKeyframe, std::vector<std::vector<glm::vec3>>& keyframeVertices, std::vector<std::vector<glm::vec3>>& keyframeVertexNormals, std::vector<std::vector<glm::vec2>>& keyframeTexCoords, std::vector<std::vector<std::string>>& faceElementLines, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor);

	/// @brief Given keyframe vertices and other data, populate tweenFrameVertices with generated vertex data.
	void generateTweenFrameVertices(std::vector<std::vector<glm::vec3>>& tweenFrameVertices, const std::vector<std::vector<glm::vec3>>& keyframeVertices, size_t framesPerMesh, size_t currentKeyframe);

	/// @brief Given facelinetokens, generate face elements that represent the tokens
	void mapFaceElementData(std::vector<FaceElement>& faces,const std::vector<std::vector<std::string>>& faceLineTokens);

	/// @brief Given faces for a frame, populate the m_frameVertices and indices.
	void storeFaceElements(
		size_t frameIndex,
		const std::vector<FaceElement>& faceElements,
		const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec2>& textureCoordinates,
		const std::vector<glm::vec3>& vertexNormals);

	/// @brief Given prefabricated vertices for a rectangle, populate the m_frameVertices and indices for the specified frame
	void storeRectangleVertices(
		size_t animationIndex,
		const Vertex& topRight,
		const Vertex& bottomRight,
		const Vertex& bottomLeft,
		const Vertex& topLeft
	);

	size_t m_activeMeshIndex = 0;

	std::vector<std::vector<GLuint>> m_frameIndices;
	std::vector<std::vector<Vertex>> m_frameVertices;
	bool m_usesOrthographicProjection = false;
	bool m_animated = false;
};

#endif
