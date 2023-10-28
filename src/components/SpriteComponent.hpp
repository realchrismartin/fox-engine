#ifndef GLSPRITECOMPONENT_HPP
#define GLSPRITECOMPONENT_HPP

class PhysicsComponent;

class SpriteComponent
{
public:
	void updateWithPhysics(const PhysicsComponent& component);
	const std::vector<GLfloat>& getVertices() const;
	const std::vector<GLuint>& getIndices() const;
	size_t getVertexCount() const;
	size_t getIndexCount() const;

private:
	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;
	sf::Vector2i m_textureCoordinates = sf::Vector2i(0, 0);
};

#endif
