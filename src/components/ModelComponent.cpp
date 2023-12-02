#include "src/components/ModelComponent.hpp"

#include "src/graphics/ModelConfig.hpp"

#include <fstream>
#include <sstream>

size_t ModelComponent::getNumMeshes() const
{
	//NB: assumes the number of meshes is constant across vertices,indices lists
	return m_meshVertices.size();
}

size_t ModelComponent::getActiveMeshIndex() const
{
	return m_activeMeshIndex;
}

void ModelComponent::setActiveMesh(size_t meshIndex)
{
	//NB: assumes the number of meshes is constant across vertices,indices lists
	if (meshIndex < 0 || meshIndex >= m_meshVertices.size())
	{
		return;
	}

	m_activeMeshIndex = meshIndex;
}

size_t ModelComponent::getVertexCount() const
{
	return m_meshVertices[m_activeMeshIndex].size();
}

size_t ModelComponent::getIndexCount() const
{
	return m_meshIndices[m_activeMeshIndex].size();
}

const std::vector<GLuint>& ModelComponent::getIndices() const
{
	return m_meshIndices[m_activeMeshIndex];
}

const std::vector<Vertex>& ModelComponent::getVertices() const
{
	return m_meshVertices[m_activeMeshIndex];
}

void ModelComponent::setTransformPoolIndex(size_t transformPoolIndex)
{
	for (size_t i = 0; i < m_meshVertices.size(); i++)
	{
		for (size_t j = 0; j < m_meshVertices[i].size(); j++)
		{
			m_meshVertices[i][j].mvpIndex = (GLfloat)transformPoolIndex;
		}
	}
}

void ModelComponent::loadModel(const ModelConfig& modelData)
{
	if (modelData.meshes.empty())
	{
		return;
	}

	m_activeMeshIndex = 0; //Reset!
	m_meshIndices.clear();
	m_meshVertices.clear();

	//Find the ratio by which we will be adjusting the model's texture coordinates to fit the mesh, since the sprite is going to be smaller than the texture atlas (it's IN the texture atlas)
	glm::vec2 textureCoordinateRatio = glm::vec2((float)modelData.spriteSize.x / (float)modelData.textureSize.x, (float)modelData.spriteSize.y / (float)modelData.textureSize.y);

	//Find the amount to add to each texture coordinate to offset it correctly in the overall texture
	glm::vec2 textureOffsetFactor = glm::vec2((float)modelData.spriteOffsetOnTexture.x / (float)modelData.textureSize.x, (float)modelData.spriteOffsetOnTexture.y / (float)modelData.textureSize.y);
	
	//Ensure frame count is >= mesh count
	size_t frameCount = modelData.meshes.size() > modelData.frameCount ? modelData.meshes.size() : modelData.frameCount;

	size_t framesPerMesh = (float)frameCount / (float)modelData.meshes.size();

	if (framesPerMesh < 1)
	{
		Logger::log("skipping load, too few frames per mesh derived from frame count and meshes vec size.");
		return;
	}

	size_t keyframeIndex = 0;
	for (auto const& meshData : modelData.meshes)
	{
		loadKeyframe(keyframeIndex, modelData.meshes.size(), framesPerMesh, meshData, textureCoordinateRatio, textureOffsetFactor);
		keyframeIndex++;
	}
}

void ModelComponent::loadKeyframe(size_t currentKeyframe, size_t keyframeCount, size_t framesPerMesh, const MeshConfig& meshData, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor)
{
	std::ifstream input;
	input.open(meshData.meshFilePath);

	if (!input.is_open())
	{
		return;
	}

	std::string currentLine;
	std::vector<std::vector<std::string>> faceLineTokens;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec2> textureCoordinates;

	bool first = true;
	int lineType = -1;
	std::vector<std::string> lineTokens;

	while (std::getline(input, currentLine))
	{
		std::stringstream stream(currentLine);
		std::string token;

		first = true;
		lineType = -1;
		lineTokens.clear();

		while (std::getline(stream, token, ' '))
		{
			// Determine line type
			if (first)
			{
				if (token == "v")
				{
					lineType = 0;
				}
				else if (token == "vt")
				{
					lineType = 1;
				}
				else if (token == "vn")
				{
					lineType = 2;
				}
				else if (token == "f")
				{
					lineType = 3;
				}

				first = false;
				continue;
			}

			if (lineType != -1)
			{
				lineTokens.push_back(token);
			}
		}

		switch (lineType)
		{
		case (0): 
			if (lineTokens.size() != (size_t)3)
			{
				break;
			}
			vertices.push_back(glm::vec3((float)std::stof(lineTokens[0]), (float)std::stof(lineTokens[1]), (float)std::stof(lineTokens[2])));
			break;
		case (1):                                
		{
			if (lineTokens.size() != (size_t)2)
			{
				break;
			}

			float s = (float)std::stof(lineTokens[0]);
			float t = (float)std::stof(lineTokens[1]);

			textureCoordinates.push_back(glm::vec2(textureOffsetFactor.x + (s * textureCoordinateRatio.x), textureOffsetFactor.y + (t * textureCoordinateRatio.y)));
			break;
		}
		case(2):
		{
			//Vertex normal
			if (lineTokens.size() != (size_t)3)
			{
				break;
			}
			vertexNormals.push_back(glm::vec3((float)std::stof(lineTokens[0]), (float)std::stof(lineTokens[1]), (float)std::stof(lineTokens[2])));
			break;
		}
		case (3): 
			faceLineTokens.push_back(lineTokens); 
			break;
		default:
			break;
		}
	}

	input.close();

	std::map<std::string, GLuint> faceMap;

	// Now that other data is loaded, load faces and update indices/vertices.

	for (size_t i = 0; i < framesPerMesh; i++)
	{
		m_meshIndices.push_back({});
		m_meshVertices.push_back({});

		if (currentKeyframe <= 0)
		{
			//dont add more frames if not lerping
			break;
		}
	}

	for (auto const& faceString : faceLineTokens)
	{
		//Load the data in to the LAST index in m_meshVertices and m_meshIndices.
		//Then we add extra data in between below if needed.
		loadFace(m_meshVertices.size() - 1, vertices, textureCoordinates, vertexNormals, faceString, faceMap);
	}

	//The keyframe is loaded in indices and vertices lists.

	if (framesPerMesh <= 1)
	{
		//Safety.
		return;
	}

	if (currentKeyframe <= 0)
	{
		//This is the first keyframe. nothing to lerp
		return;
	}

	size_t targetKeyframe = currentKeyframe * framesPerMesh; //The frame we just added.
	size_t priorKeyframe = currentKeyframe == 1 ? 0 : (currentKeyframe - 1) * framesPerMesh; //The prior keyframe

	if (m_meshVertices.at(priorKeyframe).size() != m_meshVertices.at(targetKeyframe).size())
	{
		Logger::log("Something is wrong with lerping, target has different verts size than prior. Skipping frame addition");
		return;
	}

	if (m_meshIndices.at(priorKeyframe).size() != m_meshIndices.at(targetKeyframe).size())
	{
		Logger::log("Something is wrong with lerping, index list sizes differ. Skipping frame addition");
		return;
	}
	
	for (size_t lerpMeshIndex = currentKeyframe; lerpMeshIndex < targetKeyframe; lerpMeshIndex++)
	{
		//Indices are unchanged and copied from keyframe we're expanding
		for (auto const& index : m_meshIndices.at(priorKeyframe))
		{
			m_meshIndices.at(lerpMeshIndex).push_back(index);
		}

		GLfloat factor = 0.f;
		GLfloat increase = 1.f / (GLfloat)framesPerMesh;


		//TODO: this wont work as is because faces reuse verts, and we dont!!! need to convert back into obj ish format??

		//Vertices are lerped prior -> target
		for (size_t vertexIndex = 0; vertexIndex < m_meshVertices.at(priorKeyframe).size(); vertexIndex++)
		{
			//Get ref to the target vertex
			Vertex& src = m_meshVertices.at(priorKeyframe).at(vertexIndex);
			Vertex& target = m_meshVertices.at(targetKeyframe).at(vertexIndex);

			//Create a copy of the prior vertex
			Vertex copy = m_meshVertices.at(priorKeyframe).at(vertexIndex);

			//Adjust the vertex bits in the copy according to the lerp
			copy.x = std::lerp(src.x, target.x, factor);
			copy.y = std::lerp(src.y, target.y,factor);
			copy.z = std::lerp(src.z, target.z,factor);

			//Add the copy
			m_meshVertices.at(lerpMeshIndex).push_back(copy);

			factor += increase;
		}
	}
}

void ModelComponent::loadFace(
	size_t meshIndex,
	const std::vector<glm::vec3>& vertices, 
	const std::vector<glm::vec2>& textureCoordinates, 
	const std::vector<glm::vec3>& vertexNormals, 
	const std::vector<std::string>& faceData, std::map<std::string,GLuint>& faceMap)
{
	size_t vertexCount = 0;

	for (auto const& faceComponent : faceData)
	{
		std::stringstream stream(faceComponent);
		std::string token;

		int dataType = 0; // Whether this token is a vertex index, vertex texture index

		int vertexIndex = -1;
		int vertexTextureIndex = -1;
		int vertexNormalIndex = -1;

		//Faces are defined using lists of vertex, textureand normal indices in the format vertex_index / texture_index / normal_index for which each index starts at 1
		while (std::getline(stream, token, '/'))
		{
			// Get the index for whatever data token this is, decrement by 1 due to 1-indexing
			int index = (int)atoi(token.c_str());

			switch (dataType)
			{
			case (0): // Vertex
				vertexIndex = index;
				break;
			case (1): // Vertex texture
				vertexTextureIndex = index;
				break;
			case(2): //Vertex normal
				vertexNormalIndex = index;
				break;
			default: // Unused / extra data
				break;
			}

			dataType++;
		}

		if ((vertexIndex -1) >= vertices.size())
		{
			std::cout << "ERROR: tried to load vertex " << vertexIndex << " but there were only " << vertices.size() << " vertices!!" << std::endl;
			return;
		}

		if ((vertexTextureIndex - 1) >= textureCoordinates.size())
		{
			std::cout << "ERROR: tried to load texture " << vertexTextureIndex << " but there were only " << textureCoordinates.size() << " texture coords!!" << std::endl;
			return;
		}

		// Create a map key from the assembled data for this face - look up the data from the model and connect it together
		// Store the data in the map (if needed) and push vertices if needed, store an index either way
		std::string mapKey = std::to_string(vertexIndex) + "_" + std::to_string(vertexTextureIndex) + "_" + std::to_string(vertexNormalIndex);

		if (faceMap.count(mapKey))
		{
			m_meshIndices[meshIndex].push_back(faceMap.at(mapKey));
		}
		else
		{
			// We need to push both vertex data and index data
			// We also need to update our map
			auto v = vertices.at(vertexIndex - 1);
			auto t = textureCoordinates.at(vertexTextureIndex - 1);

			// Add vertex data to vertex vector
			// Note: ordering we push here needs to match the buffer layout

			Vertex vertex;
			vertex.x = v.x;
			vertex.y = v.y;
			vertex.z = v.z;
			vertex.s = t.x;
			vertex.t = t.y;
			vertex.mvpIndex = 0.f; //This is updated as soon as this method completes in void Scene::updateAllModelComponentAssociations()

			m_meshVertices[meshIndex].push_back(vertex);

			GLuint index = (GLuint)faceMap.size();    // This is the "next" face - first one is index 0, etc.
			faceMap[mapKey] = index;          // Update map to specify index for this face - map size changes, so next index is + 1
			m_meshIndices[meshIndex].push_back(index);
		}
	}
}
