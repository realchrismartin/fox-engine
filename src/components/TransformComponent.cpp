#include "src/components/TransformComponent.hpp"

glm::mat4 TransformComponent::getLocalMatrix()
{
	glm::mat4 matrix = glm::mat4(1.0);
	matrix = glm::translate(matrix, m_translation);
	matrix = glm::rotate(matrix, m_rotation.x, glm::vec3(1.f, 0.f, 0.f));
	matrix = glm::rotate(matrix, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
	matrix = glm::rotate(matrix, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
	matrix = glm::scale(matrix, m_scale);

	return matrix;
}

const glm::mat4& TransformComponent::getWorldMatrix() const
{
	return m_worldMatrix;
}

void TransformComponent::updateLocalAndWorldMatrix()
{
	if (m_localDirty)
	{
		m_localMatrix = getLocalMatrix();
		m_localDirty = false;

		//If we updated the local matrix, update the world matrix too
		m_worldMatrix = m_localMatrix;
		m_worldDirty = true;
	}
}

void TransformComponent::updateLocalAndWorldMatrix(const TransformComponent& parentComponent)
{
	bool localDirty = m_localDirty;

	//Update the local matrix if it's dirty
	if (m_localDirty)
	{
		m_localMatrix = getLocalMatrix();
		m_localDirty = false;
	}
	
	//If we updated the local matrix, or the parent's world matrix is dirty, update the world matrix too
	if (localDirty || parentComponent.isWorldMatrixDirty())
	{
		m_worldMatrix = parentComponent.getWorldMatrix() * m_localMatrix;
		m_worldDirty = true; //If the parent's matrix is dirty, it means this transform's matrix is now too.
	}
}

void TransformComponent::updateOtherTransformLocalsToMatchThis(TransformComponent& otherTransform)
{
	otherTransform.setScale(m_scale);
	otherTransform.setTranslation(m_translation);
	otherTransform.setRotation(m_rotation);
}

void TransformComponent::markWorldMatrixClean()
{
	m_worldDirty = false;
}

bool TransformComponent::isWorldMatrixDirty() const
{
	return m_worldDirty;
}

void TransformComponent::setRotation(glm::vec3 rotation)
{
	//TODO: negative rotations don't work here. change them to positive 
	float xFactor = rotation.x > 0.f ? 1.f : -1.f;
	float yFactor = rotation.y > 0.f ? 1.f : -1.f;
	float zFactor = rotation.z > 0.f ? 1.f : -1.f;

	m_rotation = glm::vec3(glm::radians(xFactor * fmod(rotation.x,360.f)), glm::radians(yFactor * fmod(rotation.y,360.f)), glm::radians(zFactor * fmod(rotation.z,360.f)));
	m_localDirty = true;
}

void TransformComponent::setScale(glm::vec3 scale)
{
	m_scale = glm::vec3(std::fmax(scale.x, 0.f), std::fmax(scale.y, 0.f), std::fmax(scale.z, 0.f));
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

void TransformComponent::reset()
{
	m_rotation = { 0.f,0.f,0.f };
	m_scale = { 1.f,1.f,1.f };
	m_translation = { 0.f,0.f,0.f };
	m_localDirty = true;
}
