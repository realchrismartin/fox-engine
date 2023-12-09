#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/matrix_clip_space.hpp"

class Scene;

/// @brief The game's camera.
class Camera
{
public:
	/// @brief Returns true if the view matrix changed during the camera's last update.
	/// @return 
	bool isViewMatrixDirty() const;

	/// @brief Returns true if the projection matrix changed during the camera's last update
	/// @return 
	bool isProjectionMatrixDirty() const;

	/// @brief Indicate that the view matrix update is completed.
	void markViewMatrixClean();

	/// @brief Indicate that the projection matrix update is completed.
	void markProjectionMatrixClean();

	/// @brief Get the current projection matrix 
	/// @return 
	const glm::mat4& getProjectionMatrix() const;

	/// @brief Get the current view matrix
	/// @return 
	const glm::mat4& getViewMatrix() const;

	/// @brief Update the projection matrix, possibly marking it dirty if it needs to change.
	void updateProjectionMatrix();

	/// @brief Update the view matrix, possibly marking it dirty if it needs to change
	/// @param scene The scene from which data will be used to update the View matrix, depending on what the camera wants to look at.
	void updateViewMatrix(Scene& scene);

	/// @brief Tell the camera if the scene changed.
	void informOfSceneChange();

private:

	glm::mat4 m_viewMatrix = glm::mat4(1.f);
	glm::mat4 m_projectionMatrix = glm::mat4(1.f);

	bool m_viewMatrixEverSet = false;
	bool m_projectionMatrixEverSet = false;

	bool m_viewMatrixDirty = true;
	bool m_projectionMatrixDirty = true;
	bool m_orthographicProjectionMatrixDirty = true;

	std::optional<int> m_cameraEntityLastTick = std::nullopt;
	std::optional<int> m_cameraTargetLastTick = std::nullopt;

	glm::vec3 m_cameraCenter = glm::vec3(0.f,10.f,-10.f);
	glm::vec3 m_cameraTarget = glm::vec3(0.f, 0.f, 0.f);

	const glm::vec3 m_defaultCameraCenter = glm::vec3(0.f,10.f,-10.f);
	const glm::vec3 m_defaultCameraTarget = glm::vec3(0.f, 0.f, 0.f);
	const glm::vec3 m_defaultCameraUpVector = glm::vec3(0.f, 1.f, 0.f);

};

#endif