#include "ModelComponent.hpp"

#include <fstream>
#include <sstream>

ModelComponent::ModelComponent()
{
    loadModel(m_modelData); //TODO: pointless copy
}

void ModelComponent::loadModel(const ModelData& model)
{
    m_vertices.clear();
    m_indices.clear();

    m_modelData = model;

    std::ifstream input;
    input.open(m_modelData.modelFilePath);

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
			textureCoordinates.push_back(glm::vec2((float)std::stof(lineTokens[0]), (float)std::stof(lineTokens[1])));
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

    glm::vec2 textureCoordinateRatio = glm::vec2((float)m_modelData.spriteSize.x / (float)m_modelData.textureSize.x, (float)m_modelData.spriteSize.y / (float)m_modelData.textureSize.y);

    //Find the amount to add to each texture coordinate to offset it correctly in the overall texture
    glm::vec2 textureOffsetFactor = glm::vec2((float)m_modelData.spriteOffsetOnTexture.x / (float)m_modelData.textureSize.x, (float)m_modelData.spriteOffsetOnTexture.y / (float)m_modelData.textureSize.y);

    // Now that other data is loaded, load faces
    for (auto const& faceString : faceLineTokens)
    {
        loadFace(vertices, textureCoordinates, vertexNormals, faceString, faceMap, textureCoordinateRatio, textureOffsetFactor);
    }
}

void ModelComponent::loadFace(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& textureCoordinates, const std::vector<glm::vec3>& vertexNormals, const std::vector<std::string>& faceData, std::map<std::string,GLuint>& faceMap, const glm::vec2& textureCoordinateRatio, const glm::vec2& textureOffsetFactor)
{
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
            // Map doesn't need to be updated
            m_indices.push_back(faceMap.at(mapKey)); // Add face index to buffer
        }
        else
        {
            // We need to push both vertex data and index data
            // We also need to update our map
            auto v = vertices.at(vertexIndex - 1);
            auto t = textureCoordinates.at(vertexTextureIndex - 1);

            // Add vertex data to buffer
            // Note: ordering we push here needs to match the buffer layout
            m_vertices.push_back(v.z);
            m_vertices.push_back(v.y);
            m_vertices.push_back(v.x);
            m_vertices.push_back(textureOffsetFactor.x + (t.x * textureCoordinateRatio.x));
            m_vertices.push_back(textureOffsetFactor.y + (t.y * textureCoordinateRatio.y));

            GLuint index = (GLuint)faceMap.size();    // This is the "next" face - first one is index 0, etc.
            faceMap[mapKey] = index;          // Update map to specify index for this face - map size changes, so next index is + 1
            m_indices.push_back(index); // Add face index 
        }
    }
}

const std::vector<GLfloat>& ModelComponent::getVertices() const
{
	return m_vertices;
}

const std::vector<GLuint>& ModelComponent::getIndices() const
{
	return m_indices;
}
