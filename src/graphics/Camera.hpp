#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/matrix_clip_space.hpp"

class Scene;

class Camera
{
public:
	bool isViewMatrixDirty() const;
	void markViewMatrixClean();

	bool isPerspectiveProjectionMatrixDirty() const;
	bool isOrthographicProjectionMatrixDirty() const;

	const glm::mat4& getPerspectiveProjectionMatrix() const;
	const glm::mat4& getOrthographicProjectionMatrix() const;
	const glm::mat4& getViewMatrix() const;

	void updatePerspectiveProjectionMatrix(const glm::i64vec2& windowSize);
	void updateOrthographicProjectionMatrix(const glm::i64vec2& windowSize);
	void updateViewMatrix(Scene& scene);

	void informOfSceneChange();
private:

	glm::mat4 m_viewMatrix = glm::mat4(1.f);
	glm::mat4 m_perspectiveProjectionMatrix = glm::mat4(1.f); //The matrix used for all things in the world that render using perspective
	glm::mat4 m_orthographicProjectionMatrix = glm::mat4(1.f); //The matrix used for all UI elements that render orthographically

 
	bool m_viewMatrixEverSet = false;
	bool m_viewMatrixDirty = true;

	std::optional<int> m_cameraEntityLastTick = std::nullopt;
	std::optional<int> m_cameraTargetLastTick = std::nullopt;

	glm::vec3 m_cameraCenter = glm::vec3(0.f,10.f,-10.f);
	glm::vec3 m_cameraTarget = glm::vec3(0.f, 0.f, 0.f);

	const glm::vec3 m_defaultCameraCenter = glm::vec3(0.f,10.f,-10.f);
	const glm::vec3 m_defaultCameraTarget = glm::vec3(0.f, 0.f, 0.f);
	const glm::vec3 m_defaultCameraUpVector = glm::vec3(0.f, 1.f, 0.f);

};

#endif