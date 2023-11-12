#include "src/components/WorldTransformComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

const glm::mat4& WorldTransformComponent::getWorldMatrix() const 
{
	return m_worldMatrix;
}

void WorldTransformComponent::setWorldMatrix(const glm::mat4& worldMatrix)
{
	m_worldMatrix = worldMatrix;
}
