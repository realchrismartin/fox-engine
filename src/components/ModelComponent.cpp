#include "src/components/ModelComponent.hpp"

#include "src/components/config/ModelConfig.hpp"
#include "src/components/config/TextConfig.hpp"
#include "src/util/FontMapper.hpp"

#include <fstream>
#include <sstream>

size_t ModelComponent::getNumMeshes() const
{
	//NB: assumes the number of meshes is constant across vertices,indices lists
	return m_frameVertices.size();
}

size_t ModelComponent::getActiveMeshIndex() const
{
	return m_activeMeshIndex;
}

void ModelComponent::setActiveMesh(size_t meshIndex)
{
	//NB: assumes the number of meshes is constant across vertices,indices lists
	if (meshIndex < 0 || meshIndex >= m_frameVertices.size())
	{
		return;
	}

	m_activeMeshIndex = meshIndex;
}

size_t ModelComponent::getVertexCount() const
{
	return m_frameVertices[m_activeMeshIndex].size();
}

size_t ModelComponent::getIndexCount() const
{
	return m_frameIndices[m_activeMeshIndex].size();
}

const std::vector<GLuint>& ModelComponent::getIndices() const
{
	return m_frameIndices[m_activeMeshIndex];
}

const std::vector<Vertex>& ModelComponent::getVertices() const
{
	return m_frameVertices[m_activeMeshIndex];
}

void ModelComponent::setTransformPoolIndex(size_t transformPoolIndex)
{
	for (size_t i = 0; i < m_frameVertices.size(); i++)
	{
		for (size_t j = 0; j < m_frameVertices[i].size(); j++)
		{
			m_frameVertices[i][j].mvpIndex = (GLfloat)transformPoolIndex;
		}
	}
}

bool ModelComponent::usesOrthographicProjection() const
{
	return m_usesOrthographicProjection;
}

void ModelComponent::loadText(const TextConfig& textConfig)
{
	if (!m_frameVertices.empty() || !m_frameIndices.empty())
	{
		Logger::log("Model is already loaded, cannot load text.");
		return;
	}

	if (textConfig.textToDisplay.size() <= 0)
	{
		Logger::log("There is no text to display! Cannot load text.");
		return;
	}

	m_usesOrthographicProjection = true;

	m_frameVertices.push_back({});
	m_frameIndices.push_back({});

	//TODO: move elsewhere
	constexpr glm::vec2 textureSize = glm::vec2(2500.f, 2500.f); //TODO
	constexpr unsigned int screenXSize = 800;

	float widest = 0.f;

	for (size_t characterIndex = 0; characterIndex < textConfig.textToDisplay.size(); characterIndex++)
	{
		glm::vec2 size = FontMapper::getCharacterSpriteSize(textConfig.textToDisplay[characterIndex]);

		widest = widest > size.x ? widest : size.x;
	}

	//How many character can fit on the screen?
	unsigned int maxCharactersX = screenXSize / (unsigned int)std::round(widest);

	float widestCharScreenWidth = (2.f / textConfig.fontSize) / maxCharactersX;
	float charHeight = .1f; //TODO

	float leftCharacterBound = 0.f;

	size_t line = textConfig.maxLines;
	int lineCounter = textConfig.charactersPerLine;

	for (size_t characterIndex = 0; characterIndex < textConfig.textToDisplay.size(); characterIndex++)
	{
		glm::vec2 characterSize = FontMapper::getCharacterSpriteSize(textConfig.textToDisplay[characterIndex]);
		glm::vec2 characterTexCoords = FontMapper::getCharacterTexCoords(textConfig.textToDisplay[characterIndex]); //Analogous to the sprite offset on a texture

		glm::vec2 textureOffsetFactor = glm::vec2((float)(characterTexCoords.x) / (float)(textureSize.x), (float)(textureSize.y - characterSize.y - characterTexCoords.y) / (float)(textureSize.y));
		glm::vec2 characterSizeRatio = glm::vec2((float)characterSize.x / (float)textureSize.x, (float)characterSize.y / (float)textureSize.y);

		float charWidthRatio = characterSize.x / widest;

		float rightCharacterBound = leftCharacterBound + (widestCharScreenWidth * charWidthRatio);

		Vertex topRight;
		topRight.x = rightCharacterBound;
		topRight.y = charHeight * (line + 1);
		topRight.z = 0.f;
		topRight.s = textureOffsetFactor.x + characterSizeRatio.x;
		topRight.t = textureOffsetFactor.y + characterSizeRatio.y;

		Vertex bottomRight;
		bottomRight.x = rightCharacterBound;
		bottomRight.y = charHeight * line;
		bottomRight.z = 0.f;
		bottomRight.s = textureOffsetFactor.x + characterSizeRatio.x;
		bottomRight.t = textureOffsetFactor.y;

		Vertex bottomLeft;
		bottomLeft.x = leftCharacterBound;
		bottomLeft.y = charHeight * line;
		bottomLeft.z = 0.f;
		bottomLeft.s = textureOffsetFactor.x;
		bottomLeft.t = textureOffsetFactor.y;

		Vertex topLeft;
		topLeft.x = leftCharacterBound;
		topLeft.y = charHeight * (line + 1);
		topLeft.z = 0.f;
		topLeft.s = textureOffsetFactor.x;
		topLeft.t = textureOffsetFactor.y + characterSizeRatio.y;

		m_frameVertices[0].push_back(topRight);
		m_frameVertices[0].push_back(bottomRight);
		m_frameVertices[0].push_back(bottomLeft);
		m_frameVertices[0].push_back(topLeft);

		size_t baseIndex = 4 * characterIndex;

		m_frameIndices[0].push_back(baseIndex); //0
		m_frameIndices[0].push_back(baseIndex + 1); //0
		m_frameIndices[0].push_back(baseIndex + 3); //0
		m_frameIndices[0].push_back(baseIndex + 1); //0
		m_frameIndices[0].push_back(baseIndex + 2); //0
		m_frameIndices[0].push_back(baseIndex+ 3); //0

		leftCharacterBound += (widestCharScreenWidth * charWidthRatio);

		if (lineCounter >= 0)
		{
			lineCounter--;
		}

		if (lineCounter < 0)
		{
			if (textConfig.textToDisplay[characterIndex] == ' ')
			{
				lineCounter = textConfig.charactersPerLine;
				line--;
				leftCharacterBound = 0.f;

				if (line < 0)
				{
					//Too long!
					break;
				}
			}
		}
	}
}

void ModelComponent::loadModel(const ModelConfig& modelData)
{
	if (modelData.keyframeFilePaths.empty())
	{
		return;
	}

	if (!m_frameVertices.empty() || !m_frameIndices.empty())
	{
		Logger::log("Model is already loaded, cannot load again.");
		return;
	}

	m_activeMeshIndex = 0; //Reset!
	m_frameIndices.clear();
	m_frameVertices.clear();

	//Find the ratio by which we will be adjusting the model's texture coordinates to fit the mesh, since the sprite is going to be smaller than the texture atlas (it's IN the texture atlas)
	glm::vec2 textureCoordinateRatio = glm::vec2((float)modelData.spriteSize.x / (float)modelData.textureSize.x, (float)modelData.spriteSize.y / (float)modelData.textureSize.y);

	//Find the amount to add to each texture coordinate to offset it correctly in the overall texture
	//NB: this assumes the offset is starting from the top left, + on the Y axis goes down
	glm::vec2 textureOffsetFactor = glm::vec2((float)(modelData.spriteOffsetOnTexture.x) / (float)(modelData.textureSize.x), (float)(modelData.textureSize.y - modelData.spriteSize.y - modelData.spriteOffsetOnTexture.y) / (float)(modelData.textureSize.y));
	
	//Ensure frame count is >= mesh count
	size_t frameCount = modelData.keyframeFilePaths.size() > modelData.frameCount ? modelData.keyframeFilePaths.size() : modelData.frameCount;

	size_t framesPerMesh = (float)frameCount / (float)modelData.keyframeFilePaths.size();

	if (framesPerMesh < 1)
	{
		Logger::log("skipping load, too few frames per mesh derived from frame count and meshes vec size.");
		return;
	}

	//Create a place we will be storing our raw OBJ datas
	std::vector<std::vector<glm::vec3>> keyframeVertices;
	std::vector<std::vector<glm::vec3>> keyframeVertexNormals;
	std::vector<std::vector<glm::vec2>> keyframeTexCoords;

	size_t keyframesToStore = modelData.keyframeFilePaths.size();

	while (keyframesToStore > 0)
	{
		keyframeVertices.push_back({});
		keyframeVertexNormals.push_back({});
		keyframeTexCoords.push_back({});
		
		keyframesToStore--;
	}

	size_t keyframeIndex = 0;

	for (auto const& keyframePath : modelData.keyframeFilePaths)
	{
		loadKeyframe(keyframeIndex, framesPerMesh, keyframePath, keyframeVertices,keyframeVertexNormals,keyframeTexCoords, textureCoordinateRatio, textureOffsetFactor);
		keyframeIndex++;
	}
}


void ModelComponent::loadKeyframe(size_t currentKeyframe, size_t framesPerMesh, const std::string& keyframePath,
	std::vector<std::vector<glm::vec3>>& keyframeVertices, std::vector<std::vector<glm::vec3>>& keyframeVertexNormals, std::vector<std::vector<glm::vec2>>& keyframeTexCoords,
	const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor)
{
	//Ensure there is at least one frame.
	framesPerMesh = framesPerMesh <= 0 ? 1 : framesPerMesh;

	//Add mesh vertices and indices vecs
	for (int i = 0; i < framesPerMesh; i++)
	{
		m_frameVertices.push_back({});
		m_frameIndices.push_back({});

		if (currentKeyframe == 0)
		{
			break;
		}
	}

	std::vector<std::vector<std::string>> faceElementLines;

	//Load the OBJ file for the keyframe, populatign the vectors.
	loadOBJFile(keyframePath, currentKeyframe, keyframeVertices, keyframeVertexNormals, keyframeTexCoords, faceElementLines, textureCoordinateRatio, textureOffsetFactor);

	std::vector<FaceElement> faceElements;

	//Load all of the faceElement data from strings into faceElements
	mapFaceElementData(faceElements, faceElementLines);

	//Generate fake vertices for the tween frames if tweening is needed
	if (currentKeyframe > 0)
	{
		std::vector<std::vector<glm::vec3>> tweenFrameVertices;

		generateTweenFrameVertices(tweenFrameVertices, keyframeVertices, framesPerMesh, currentKeyframe);

		size_t tweenFrameIndex = currentKeyframe == 1 ? 1 : (currentKeyframe * framesPerMesh) - framesPerMesh + 1;

		for (int i = 0; i < (framesPerMesh - 1); i++)
		{
			//Load the faceElements of the tween frames
			storeFaceElements(tweenFrameIndex + i, faceElements, tweenFrameVertices[i], keyframeTexCoords[currentKeyframe], keyframeVertexNormals[currentKeyframe]);
		}
	}

	//Load the faceElements of the keyframe
	storeFaceElements(currentKeyframe* framesPerMesh, faceElements, keyframeVertices[currentKeyframe], keyframeTexCoords[currentKeyframe], keyframeVertexNormals[currentKeyframe]);
}

void ModelComponent::loadOBJFile(const std::string& keyframePath, size_t currentKeyframe, 
	std::vector<std::vector<glm::vec3>>& keyframeVertices,
	std::vector<std::vector<glm::vec3>>& keyframeVertexNormals,
	std::vector<std::vector<glm::vec2>>& keyframeTexCoords, 
	std::vector<std::vector<std::string>>& faceElementLines,
	const glm::vec2& textureCoordinateRatio,
	const glm::vec2& textureOffsetFactor)
{

	std::ifstream input;
	input.open(keyframePath);

	if (!input.is_open())
	{
		return;
	}

	std::string currentLine;

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
			keyframeVertices[currentKeyframe].push_back(glm::vec3((float)std::stof(lineTokens[0]), (float)std::stof(lineTokens[1]), (float)std::stof(lineTokens[2])));
			break;
		case (1):
		{
			if (lineTokens.size() != (size_t)2)
			{
				break;
			}

			float s = (float)std::stof(lineTokens[0]);
			float t = (float)std::stof(lineTokens[1]);

			keyframeTexCoords[currentKeyframe].push_back(glm::vec2(textureOffsetFactor.x + (s * textureCoordinateRatio.x), textureOffsetFactor.y + (t * textureCoordinateRatio.y)));
			break;
		}
		case(2):
		{
			//Vertex normal
			if (lineTokens.size() != (size_t)3)
			{
				break;
			}
			keyframeVertexNormals[currentKeyframe].push_back(glm::vec3((float)std::stof(lineTokens[0]), (float)std::stof(lineTokens[1]), (float)std::stof(lineTokens[2])));
			break;
		}
		case (3):
			faceElementLines.push_back(lineTokens);
			break;
		default:
			break;
		}
	}

	input.close();
}

void ModelComponent::generateTweenFrameVertices(std::vector<std::vector<glm::vec3>>& tweenFrameVertices, const std::vector<std::vector<glm::vec3>>& keyframeVertices, size_t framesPerMesh, size_t currentKeyframe)
{
	GLfloat increase = 1.f / (GLfloat)framesPerMesh;
	GLfloat factor = 0.f;

	for (int i = 0; i < (framesPerMesh - 1); i++)
	{
		tweenFrameVertices.push_back({});

		for (size_t vertexIndex = 0; vertexIndex < keyframeVertices[currentKeyframe - 1].size(); vertexIndex++)
		{
			//Get referencs to the source and target vertex lists for lerpin
			const glm::vec3& src = keyframeVertices.at(currentKeyframe - 1)[vertexIndex];
			const glm::vec3& target = keyframeVertices.at(currentKeyframe)[vertexIndex];

			tweenFrameVertices[i].push_back({ std::lerp(src.x, target.x, factor), std::lerp(src.y, target.y, factor), std::lerp(src.z, target.z, factor) });
		}

		factor += increase;
	}
}

void ModelComponent::mapFaceElementData(std::vector<FaceElement>& faceElements, const std::vector<std::vector<std::string>>& faceElementLineTokens)
{
	for (auto const& faceElementString : faceElementLineTokens)
	{
		for (auto const& faceElement: faceElementString)
		{
			std::stringstream stream(faceElement);
			std::string token;

			int dataType = 0; // Whether this token is a vertex index, vertex texture index

			int vertexIndex = -1;
			int vertexTextureIndex = -1;
			int vertexNormalIndex = -1;

			//FaceElements are defined using lists of vertex, textureand normal indices in the format vertex_index / texture_index / normal_index for which each index starts at 1
			while (std::getline(stream, token, '/'))
			{
				// Get the index for whatever data token this is, decrement by 1 due to 1-indexing
				int index = (int)atoi(token.c_str());

				switch (dataType)
				{
				case (0): // Vertex
					vertexIndex = index - 1;
					break;
				case (1): // Vertex texture
					vertexTextureIndex = index - 1;
					break;
				case(2): //Vertex normal
					vertexNormalIndex = index - 1;
					break;
				default: // Unused / extra data
					break;
				}

				dataType++;
			}

			if (vertexIndex == -1 || vertexTextureIndex == -1 || vertexNormalIndex == -1)
			{
				Logger::log("One of the components of a faceElement is missing - model load failed.");
				return;
			}

			FaceElement faceElement;
			faceElement.vertexIndex = vertexIndex;
			faceElement.vertexTextureIndex = vertexTextureIndex;
			faceElement.vertexNormalIndex = vertexNormalIndex;
			faceElements.push_back(faceElement);
		}
	}
}

void ModelComponent::storeFaceElements(
	size_t frameIndex,
	const std::vector<FaceElement>& faceElements,
	const std::vector<glm::vec3>& vertices, 
	const std::vector<glm::vec2>& textureCoordinates, 
	const std::vector<glm::vec3>& vertexNormals)
{
	std::unordered_map<std::string, GLuint> faceElementMap;

	for (auto const& faceElement : faceElements)
	{
		// Create a map key from the assembled data for this faceElement - look up the data from the model and connect it together
		// Store the data in the map (if needed) and push vertices if needed, store an index either way
		std::string mapKey = std::to_string(faceElement.vertexIndex) + "_" + std::to_string(faceElement.vertexTextureIndex) + "_" + std::to_string(faceElement.vertexNormalIndex);

		if (faceElementMap.count(mapKey))
		{
			m_frameIndices[frameIndex].push_back(faceElementMap.at(mapKey));
		}
		else
		{
			// We need to push both vertex data and index data
			// We also need to update our map
			auto v = vertices.at(faceElement.vertexIndex);
			auto t = textureCoordinates.at(faceElement.vertexTextureIndex);

			// Add vertex data to vertex vector
			// Note: ordering we push here needs to match the buffer layout

			Vertex vertex;
			vertex.x = v.x;
			vertex.y = v.y;
			vertex.z = v.z;
			vertex.s = t.x;
			vertex.t = t.y;
			vertex.mvpIndex = 0.f; //This is updated as soon as this method completes in void Scene::updateAllModelComponentAssociations()

			m_frameVertices[frameIndex].push_back(vertex);

			GLuint index = (GLuint)faceElementMap.size();    // This is the "next" faceElement - first one is index 0, etc.
			faceElementMap[mapKey] = index;          // Update map to specify index for this faceElement - map size changes, so next index is + 1
			m_frameIndices[frameIndex].push_back(index);
		}
	}
}
