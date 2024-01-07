#include "Camera.hpp"
#include "src/graphics/Camera.hpp"

#include "src/scenes/Scene.hpp"
#include "src/components/TransformComponent.hpp"

bool Camera::isViewMatrixDirty() const
{
	return m_viewMatrixDirty;
}

void Camera::markViewMatrixClean()
{
	m_viewMatrixDirty = false;
}

bool Camera::isPerspectiveProjectionMatrixDirty() const
{
	return true; //TODO
}

bool Camera::isOrthographicProjectionMatrixDirty() const
{
	return true; //TODO
}

const glm::mat4& Camera::getPerspectiveProjectionMatrix() const
{
	return m_perspectiveProjectionMatrix;
}

const glm::mat4& Camera::getOrthographicProjectionMatrix() const
{
	return m_orthographicProjectionMatrix;
}

void Camera::updatePerspectiveProjectionMatrix(const glm::i64vec2& windowSize)
{
	// Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	float ratio = (float)windowSize.x / (float)windowSize.y;
	m_perspectiveProjectionMatrix = glm::perspective(glm::radians(90.0f), ratio, 1.f, 100.f);
}

void Camera::updateOrthographicProjectionMatrix(const glm::i64vec2& windowSize)
{
	float ratio = (float)windowSize.x / (float)windowSize.y;
	m_orthographicProjectionMatrix = glm::ortho(-ratio,ratio,-1.f,1.f,1.f,-1.f);
}

const glm::mat4& Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

void Camera::updateViewMatrix(Scene& scene)
{
	std::optional<int> cameraEntityId = scene.getCameraEntity();
	std::optional<int> cameraTargetEntityId = scene.getCameraTargetEntity();

	bool cameraChanged = false;
	bool targetChanged = false;

	if (cameraEntityId.has_value() && scene.hasComponent<TransformComponent>(cameraEntityId.value()))
	{
		TransformComponent& cameraTransform = scene.getComponent<TransformComponent>(cameraEntityId.value());

		if (!m_cameraEntityLastTick.has_value() || m_cameraEntityLastTick.value() != cameraEntityId.value() || cameraTransform.isWorldMatrixDirty())
		{
			glm::mat4 worldMatrix = cameraTransform.getWorldMatrix();
			glm::vec4 center = glm::vec4(0.f, 0.f, 0.f, 1.f);

			auto transformedCameraCenter = worldMatrix * center;

			m_cameraCenter = glm::vec3(transformedCameraCenter.x, transformedCameraCenter.y, transformedCameraCenter.z);
			cameraChanged = true;
		}
	}

	if (cameraTargetEntityId.has_value() && scene.hasComponent<TransformComponent>(cameraTargetEntityId.value()))
	{
		TransformComponent& cameraTargetTransform = scene.getComponent<TransformComponent>(cameraTargetEntityId.value());

		if (!m_cameraTargetLastTick.has_value() || m_cameraTargetLastTick.value() != cameraTargetEntityId.value() || cameraTargetTransform.isWorldMatrixDirty())
		{
			glm::mat4 worldMatrix = cameraTargetTransform.getWorldMatrix();
			glm::vec4 target = glm::vec4(0.f, 0.f, 0.f, 1.f);

			auto transformedCameraTarget = worldMatrix * target;

			m_cameraTarget = glm::vec3(transformedCameraTarget.x, transformedCameraTarget.y, transformedCameraTarget.z);
			targetChanged = true;
		}
	}

	m_cameraEntityLastTick = cameraEntityId;
	m_cameraTargetLastTick = cameraTargetEntityId;

	if (cameraChanged || targetChanged || !m_viewMatrixEverSet)
	{
		m_viewMatrix = glm::lookAt(m_cameraCenter, m_cameraTarget, m_defaultCameraUpVector); //TODO: are these backward?

		if (!m_viewMatrixEverSet)
		{
			m_viewMatrixEverSet = true;
		}

		m_viewMatrixDirty = true;
	}
}

void Camera::informOfSceneChange()
{
	//Do things needed to change the scene. We assume the next time systems run, the camera will be updated again.
	m_viewMatrixDirty = true;
	m_cameraEntityLastTick = std::nullopt;
	m_cameraTargetLastTick = std::nullopt;

	//TODO: dirtify the projection matrices?
}
