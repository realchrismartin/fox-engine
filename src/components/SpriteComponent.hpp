#ifndef GLSPRITECOMPONENT_HPP
#define GLSPRITECOMPONENT_HPP

class PhysicsComponent;

class SpriteComponent
{
public:
	void setSizeAndCenterPoint(sf::Vector2f size, sf::Vector2f centerPoint);
	void updateWithPhysics(const PhysicsComponent& component);
	const std::vector<GLfloat>& getVertices() const;
	const std::vector<GLuint>& getIndices() const;
	size_t getVertexCount() const;
	size_t getIndexCount() const;

private:
	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;
	sf::Vector2i m_textureCoordinates = sf::Vector2i(25, 25);

	float m_textureHeight = 50.f;
	float m_textureWidth = 50.f;

	float m_spriteHeight = 50.f;
	float m_spriteWidth = 50.f;

};

#endif
