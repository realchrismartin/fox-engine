#include "ModelComponent.hpp"

#include <fstream>
#include <sstream>

ModelComponent::ModelComponent()
{
    initModelData();
}

void ModelComponent::initModelData()
{
	sf::Vector2f normalizedTextureCoordinates = sf::Vector2f((float)m_textureCoordinates.x / (float)m_textureSize.x, (float)m_textureCoordinates.y / (float)m_textureSize.y);
	sf::Vector2f normalizedSpriteSize = sf::Vector2f((float)m_spriteSize.x / (float)m_textureSize.x, (float)m_spriteSize.y / (float)m_textureSize.y); //Assumes texture is bigger than the sprite...

    sf::Vector2f tl = sf::Vector2f(normalizedTextureCoordinates.x, normalizedTextureCoordinates.y);
    sf::Vector2f bl = sf::Vector2f(normalizedTextureCoordinates.x, normalizedTextureCoordinates.y - normalizedSpriteSize.y);
    sf::Vector2f tr = sf::Vector2f(normalizedTextureCoordinates.x + normalizedSpriteSize.x, normalizedTextureCoordinates.y);
    sf::Vector2f br = sf::Vector2f(normalizedTextureCoordinates.x + normalizedSpriteSize.x, normalizedTextureCoordinates.y - normalizedSpriteSize.y);

    m_vertices = {
        -0.5f, -0.5f,  0.5f, bl.x, bl.y, // Vertex 0 (Front Bottom-Left)
         0.5f, -0.5f,  0.5f, br.x, br.y, // Vertex 1 (Front Bottom-Right)
         0.5f,  0.5f,  0.5f, tr.x, tr.y, // Vertex 2 (Front Top-Right)
        -0.5f,  0.5f,  0.5f, tl.x, tl.y, // Vertex 3 (Front Top-Left)

        -0.5f, -0.5f, -0.5f, br.x, br.y, // Vertex 4 (Back Bottom-Right)
         0.5f, -0.5f, -0.5f, bl.x, bl.y, // Vertex 5 (Back Bottom-Left)
         0.5f,  0.5f, -0.5f, tl.x, tl.y, // Vertex 6 (Back Top-Left)
        -0.5f,  0.5f, -0.5f, tr.x, tr.y  // Vertex 7 (Back Top-Right)
    };

    m_indices = {
        0, 1, 2, 2, 3, 0,    // Front face
        4, 5, 6, 6, 7, 4,    // Back face
        1, 5, 6, 6, 2, 1,    // Right face
        0, 3, 7, 7, 4, 0,    // Left face
        3, 2, 6, 6, 7, 3,    // Top face
        0, 1, 5, 5, 4, 0     // Bottom face
    };

    return; //TODO

    std::ifstream input;

    input.open("../../img/cube.obj"); //TODO


    if (!input.is_open())
    {
        return;
    }

    std::string currentLine;
    std::vector<std::vector<std::string>> faceLineTokens;

    std::vector<glm::vec3> vertices;
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
				else if (token == "f")
				{
					lineType = 2;
				}

				first = false;
				continue;
			}

			lineTokens.push_back(token);
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
		case (2): 
			faceLineTokens.push_back(lineTokens); 
			break;
		default:
			break;
		}
	}

    input.close();

    // Now that other data is loaded, load faces
    for (auto const& faceString : faceLineTokens)
    {
        loadFace(vertices, textureCoordinates, faceString);
    }
}

void ModelComponent::loadFace(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& textureCoordinates, const std::vector<std::string>& faceData)
{
    std::unordered_map<std::string, GLuint> faceMap;

    for (std::string face : faceData)
    {
        std::stringstream stream(face);
        std::string token;

        int dataType = 0; // Whether this token is a vertex index, vertex texture index

        int vertexIndex = -1;
        int vertexTextureIndex = -1;

        while (std::getline(stream, token, '/'))
        {
            // Get the index for whatever data token this is, decrement by 1 due to 1-indexing
            int index = (int)atoi(token.c_str()) - 1;

            switch (dataType)
            {
            case (0): // Vertex
                vertexIndex = index;
                break;
            case (1): // Vertex texture
                vertexTextureIndex = index;
                break;
            default: // Unused / extra data
                break;
            }

            dataType++;
        }

        if (vertexIndex >= vertices.size())
        {
            std::cout << "ERROR: tried to load vertex " << vertexIndex << " but there were only " << vertices.size() << " vertices!!" << std::endl;
            return;
        }

        if (vertexTextureIndex >= textureCoordinates.size())
        {
            std::cout << "ERROR: tried to load texture " << vertexTextureIndex << " but there were only " << textureCoordinates.size() << " texture coords!!" << std::endl;
            //return;
        }

        // Create a map key from the assembled data for this face - look up the data from the model and connect it together
        // Store the data in the map (if needed) and push vertices if needed, store an index either way
        std::string mapKey = std::to_string(vertexIndex) + "_" + std::to_string(vertexTextureIndex);

        if (faceMap.count(mapKey))
        {
            // Map doesn't need to be updated
            m_indices.push_back(faceMap.at(mapKey)); // Add face index to buffer
        }
        else
        {
            // We need to push both vertex data and index data
            // We also need to update our map
            auto v = vertices.at(vertexIndex);
           // auto t = textureCoordinates.at(vertexTextureIndex);

            // Add vertex data to buffer
            // Note: ordering we push here needs to match the buffer layout
            m_vertices.push_back(v.z);
            m_vertices.push_back(v.y);
            m_vertices.push_back(v.x);

            m_vertices.push_back(0.f);
            m_vertices.push_back(0.f);
            /*
            m_vertices.push_back(t.x);
            m_vertices.push_back(t.y);
            */

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

void ModelComponent::setTextureCoordinates(sf::Vector2i coordinates)
{
    m_textureCoordinates = coordinates;
    initModelData();
}

void ModelComponent::setSpriteSize(sf::Vector2i spriteSize)
{
    m_spriteSize = spriteSize;
    initModelData();
}
