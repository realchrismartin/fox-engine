#include "src/components/GLSpriteComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

void GLSpriteComponent::updateWithPhysics(const PhysicsComponent& component)
{
	//TODO
	//Wherever the physics body is now, update the graphic to match.

	m_vertices = { 
				 0.5f,  0.5f, 0.0f,   1.f, 1.f,   // top right
				 0.5f, -0.5f, 0.0f,    1.f,0.f,   // bottom right
				-0.5f, -0.5f, 0.0f,    0.f,0.f,   // bottom left
				-0.5f,  0.5f, 0.0f,    0.f,1.f    // top left 
		};

	m_indices = { 0,1,3,1,2,3 };

}

const std::vector<GLfloat>& GLSpriteComponent::getVertices() const
{
	return m_vertices;
}

const std::vector<GLuint>& GLSpriteComponent::getIndices() const
{
	return m_indices;
}

size_t GLSpriteComponent::getVertexCount() const
{
	return m_vertices.size();
}

size_t GLSpriteComponent::getIndexCount() const
{
	return m_indices.size();
}
