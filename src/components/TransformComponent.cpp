#include "src/components/TransformComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

void TransformComponent::updateLocalMatrix()
{
	if (!m_dirty)
	{
		return;
	}

	m_localMatrix = glm::mat4(1.0);
	m_localMatrix = glm::translate(m_localMatrix, m_translation);
	m_localMatrix = glm::rotate(m_localMatrix, m_rotation.x,glm::vec3(1.f,0.f,0.f));
	m_localMatrix = glm::rotate(m_localMatrix, m_rotation.y,glm::vec3(0.f,1.f,0.f));
	m_localMatrix = glm::rotate(m_localMatrix, m_rotation.z,glm::vec3(0.f,0.f,1.f));
	m_localMatrix = glm::scale(m_localMatrix, m_scale);
}
void TransformComponent::updateWorldMatrix(const glm::mat4& parentWorldMatrix)
{
	updateLocalMatrix();

	//TODO: can we avoid redoing this multiplication? probably. maybe check if the parent is dirty?
	m_worldMatrix = m_localMatrix * parentWorldMatrix;
}

glm::mat4& TransformComponent::getWorldMatrix()
{
	return m_worldMatrix;
}

void TransformComponent::setRotation(glm::vec3 rotation)
{
	//TODO: bounds etc
	rotation = glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));

	m_rotation = rotation;
	m_dirty = true;
}

void TransformComponent::setScale(glm::vec3 scale)
{
	m_scale = scale;
	m_dirty = true;
}

void TransformComponent::setTranslation(glm::vec3 translation)
{
	m_translation = translation;
	m_dirty = true;
}
