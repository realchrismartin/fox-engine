#include "src/components/MVPTransformComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

void MVPTransformComponent::setMVPMatrix(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	m_mvpMatrix = projectionMatrix * viewMatrix * worldMatrix;
}

const glm::mat4& MVPTransformComponent::getMVPMatrix() const
{
	return m_mvpMatrix;
}
