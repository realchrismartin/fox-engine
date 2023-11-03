#ifndef MODELCOMPONENT_HPP
#define MODELCOMPONENT_HPP

class ModelComponent
{
public:
	ModelComponent();
	const std::vector<GLfloat>& getVertices() const;
	const std::vector<GLuint>& getIndices() const;
	void setTextureCoordinates(sf::Vector2i coordinates);
	void setSpriteSize(sf::Vector2i spriteSize);
private:
	void initModelData();


	void loadFace(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& textureCoordinates, const std::vector<std::string>& faceData);

	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;

	sf::Vector2i m_textureCoordinates = sf::Vector2i(0,0);

	sf::Vector2i m_textureSize = sf::Vector2i(100, 100); //TODO unhardcode
	sf::Vector2i m_spriteSize = sf::Vector2i(50,50); //TODO unhardcode
};

#endif