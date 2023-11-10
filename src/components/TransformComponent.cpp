#include "src/components/TransformComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

void TransformComponent::updateLocalMatrix()
{
	m_matrix = glm::mat4(1.0); //TODO?
	m_matrix = glm::translate(m_matrix, m_position);
	m_matrix = glm::rotate(m_matrix, m_rotation.x,glm::vec3(1.f,0.f,0.f));
	m_matrix = glm::rotate(m_matrix, m_rotation.y,glm::vec3(0.f,1.f,0.f));
	m_matrix = glm::rotate(m_matrix, m_rotation.z,glm::vec3(0.f,0.f,1.f));
	m_matrix = glm::scale(m_matrix, m_scale);
}
void TransformComponent::multiplyModelMatrix(const glm::mat4& matrix)
{
	m_matrix *= matrix; //TODO
}

glm::mat4& TransformComponent::getModelMatrix()
{
	return m_matrix;
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
