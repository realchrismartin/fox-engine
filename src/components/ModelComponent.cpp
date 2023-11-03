#include "ModelComponent.hpp"

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
