#include "src/components/TransformComponent.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

void TransformComponent::updateLocalMatrix()
{
	if (!m_localDirty)
	{
		return;
	}

	//TODO: order is probably wrong
	m_localMatrix = glm::mat4(1.0);
	m_localMatrix = glm::translate(m_localMatrix, m_translation);
	m_localMatrix = glm::scale(m_localMatrix, m_scale);
	m_localMatrix = glm::rotate(m_localMatrix, m_rotation.x,glm::vec3(1.f,0.f,0.f));
	m_localMatrix = glm::rotate(m_localMatrix, m_rotation.y,glm::vec3(0.f,1.f,0.f));
	m_localMatrix = glm::rotate(m_localMatrix, m_rotation.z,glm::vec3(0.f,0.f,1.f));

	m_localDirty = false;
}

void TransformComponent::updateLocalAndWorldMatrix()
{
	bool localDirty = m_localDirty;

	updateLocalMatrix();
	
	if (localDirty)
	{
		m_worldMatrix = m_localMatrix;
		m_worldDirty = true; //We updated.
	}
}


void TransformComponent::updateLocalAndWorldMatrix(TransformComponent& parentComponent)
{
	bool localDirty = m_localDirty;

	updateLocalMatrix();
	
	//If we updated locally or the parent updated
	if (localDirty || parentComponent.isWorldDirty())
	{
		m_worldMatrix = parentComponent.getWorldMatrix() * m_localMatrix;
		m_worldDirty = true; //We updated.
	}
}

void TransformComponent::markWorldClean()
{
	m_worldDirty = false;
}

bool TransformComponent::isWorldDirty() const
{
	return m_worldDirty;
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
	m_localDirty = true;
}

void TransformComponent::setScale(glm::vec3 scale)
{
	m_scale = scale;
	m_localDirty = true;
}

void TransformComponent::setTranslation(glm::vec3 translation)
{
	m_translation = translation;
	m_localDirty = true;
}

void TransformComponent::addTranslation(glm::vec3 translation)
{
	m_translation += translation;
	m_localDirty = true;
}

