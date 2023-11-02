#include "src/components/TransformComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

const glm::mat4& TransformComponent::getModelMatrix()
{
	glm::mat4 matrix = glm::mat4(1.0); //TODO: stop resetting this constantly?

	matrix = glm::translate(matrix, m_position);
	matrix = glm::rotate(matrix, m_rotation.x,glm::vec3(1.f,0.f,0.f));
	matrix = glm::rotate(matrix, m_rotation.y,glm::vec3(0.f,1.f,0.f));
	matrix = glm::rotate(matrix, m_rotation.z,glm::vec3(0.f,0.f,1.f));
	matrix = glm::scale(matrix, m_scale);

	return matrix;
}

void TransformComponent::setRotation(glm::vec3 rotation)
{
	//TODO: bounds etc
	rotation = glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));

	m_rotation = rotation;
}

void TransformComponent::setScale(glm::vec3 scale)
{
	m_scale = scale;
}

void TransformComponent::setPosition(glm::vec3 position)
{
	m_position = position;
}

glm::vec3 TransformComponent::getPosition() const
{
	return m_position;
}
