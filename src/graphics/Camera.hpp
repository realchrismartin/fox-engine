#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "src/scenes/Scene.hpp"
#include "src/components/TransformComponent.hpp"

#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/matrix_clip_space.hpp"

class Camera
{
public:
	bool isViewMatrixDirty() const
	{
		return m_viewMatrixDirty;
	}
	
	bool isProjectionMatrixDirty() const
	{
		return m_projectionMatrixDirty;
	}

	void markViewMatrixClean()
	{
		m_viewMatrixDirty = false;
	}

	void markProjectionMatrixClean()
	{
		m_projectionMatrixDirty = false;
	}

	const glm::mat4& getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}
	
	const glm::mat4& getViewMatrix() const
	{
		return m_viewMatrix;
	}

	void updateProjectionMatrix()
	{
		if (m_projectionMatrixEverSet)
		{
			return;
		}

		m_projectionMatrixEverSet = true;

		// Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		m_projectionMatrix = glm::perspective(glm::radians(90.0f), 1024.f / 768.f, 1.f, 100.f);

		m_projectionMatrixDirty = true;
	}

	void updateViewMatrix(Scene& scene)
	{
		std::optional<int> cameraEntityId = scene.getCameraEntity();
		std::optional<int> cameraTargetEntityId = scene.getCameraTargetEntity();

		bool cameraChanged = false;
		bool targetChanged = false;

		if (cameraEntityId.has_value() && scene.hasComponent<TransformComponent>(cameraEntityId.value()))
		{
			TransformComponent& cameraTransform = scene.getComponent<TransformComponent>(cameraEntityId.value());

			if (m_cameraEntityLastTick.has_value() && m_cameraEntityLastTick.value() == cameraEntityId.value() && !cameraTransform.isWorldMatrixDirty())
			{
				//We don't need to update the camera center.
			}
			else 
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

			if (m_cameraTargetLastTick.has_value() && m_cameraTargetLastTick.value() == cameraTargetEntityId.value() && !cameraTargetTransform.isWorldMatrixDirty())
			{
				//We don't need to update the camera center.
			}
			else
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
			m_viewMatrix = glm::lookAt(m_cameraCenter,m_cameraTarget,m_defaultCameraUpVector); //TODO: are these backward?

			if (!m_viewMatrixEverSet)
			{
				m_viewMatrixEverSet = true;
			}

			m_viewMatrixDirty = true;
		}
	}

private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	bool m_viewMatrixEverSet = false;
	bool m_projectionMatrixEverSet = false;

	bool m_viewMatrixDirty = true;
	bool m_projectionMatrixDirty = true;

	std::optional<int> m_cameraEntityLastTick = std::nullopt;
	std::optional<int> m_cameraTargetLastTick = std::nullopt;

	glm::vec3 m_cameraCenter = glm::vec3(0.f,10.f,-10.f);
	glm::vec3 m_cameraTarget = glm::vec3(0.f, 0.f, 0.f);

	const glm::vec3 m_defaultCameraCenter = glm::vec3(0.f,10.f,-10.f);
	const glm::vec3 m_defaultCameraTarget = glm::vec3(0.f, 0.f, 0.f);
	const glm::vec3 m_defaultCameraUpVector = glm::vec3(0.f, 1.f, 0.f);

};

#endif