#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "src/scenes/Scene.hpp"
#include "src/components/TransformComponent.hpp"
#include "glm/glm/ext/matrix_transform.hpp"

class Camera
{
public:
	glm::mat4 getUIViewMatrix()
	{
		return glm::mat4(1.0); //TODO
	}

	glm::mat4 getViewMatrix(Scene& scene)
	{
		glm::vec3 cameraCenter = glm::vec3(0.f,10.f,-10.f);
		glm::vec3 eyeTarget = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 cameraUpVector = glm::vec3(0.f, 1.f, 0.f);

		std::optional<int> cameraEntityId = scene.getCameraEntity();

		if (cameraEntityId.has_value())
		{
			if (scene.hasComponent<TransformComponent>(cameraEntityId.value()))
			{
				TransformComponent& cameraTransform = scene.getComponent<TransformComponent>(cameraEntityId.value());
				
				glm::mat4 worldMatrix = cameraTransform.getWorldMatrix();
				glm::vec4 center = glm::vec4(0.f, 0.f, 0.f, 1.f);

				auto transformedCameraCenter = worldMatrix * center;

				cameraCenter = glm::vec3(transformedCameraCenter.x, transformedCameraCenter.y, transformedCameraCenter.z);
			}
		}

		std::optional<int> cameraTargetEntityId = scene.getCameraTargetEntity();

		if (cameraTargetEntityId.has_value())
		{
			if (scene.hasComponent<TransformComponent>(cameraTargetEntityId.value()))
			{
				TransformComponent& cameraTargetTransform = scene.getComponent<TransformComponent>(cameraTargetEntityId.value());
				
				glm::mat4 worldMatrix = cameraTargetTransform.getWorldMatrix();
				glm::vec4 target = glm::vec4(0.f, 0.f, 0.f, 1.f);

				auto transformedCameraTarget = worldMatrix * target;

				eyeTarget = glm::vec3(transformedCameraTarget.x, transformedCameraTarget.y, transformedCameraTarget.z);
			}
		}

		return glm::lookAt(cameraCenter,eyeTarget, cameraUpVector);
	}
};

#endif