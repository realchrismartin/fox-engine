#include "src/components/TransformComponent.hpp"

#include "src/components/PhysicsComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

const glm::mat4& TransformComponent::getModelMatrix()
{
	return m_modelMatrix;
}

void TransformComponent::updateWithPhysics(const PhysicsComponent& component)
{
	sf::Vector2f pos = component.getPosition();
	glm::vec3 translation(pos.x, pos.y, 0.f);

	m_modelMatrix = glm::mat4(1.0); //tODO: stop resetting this constantly.

	//TODO: scale and rotation
	m_modelMatrix = glm::translate(m_modelMatrix, translation);
}
