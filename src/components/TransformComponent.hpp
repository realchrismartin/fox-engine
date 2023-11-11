#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

class InputComponent;

class TransformComponent
{
public:
	void markWorldMatrixClean();

	/// @brief Returns true if the transform's world matrix is dirty 
	/// @return 
	bool isWorldMatrixDirty() const;

	/// @brief Update this transform's local and world matrix, if needed
	void updateLocalAndWorldMatrix(); 

	/// @brief Update this transform's local and world matrix with the parent's world matrix, if needed
	/// @param parentComponent The transform component of the entity that is the parent of the entity that owns this transform component
	void updateLocalAndWorldMatrix(TransformComponent& parentComponent);

	/// @brief Get the world matrix for this transform.
	/// @return 
	glm::mat4& getWorldMatrix();

	/// @brief Set the local rotation for this transform
	/// @param rotation 
	void setRotation(glm::vec3 rotation);

	/// @brief Set the local scale for this transform
	/// @param scale 
	void setScale(glm::vec3 scale);

	/// @brief Set the local translation for this transform
	/// @param translation 
	void setTranslation(glm::vec3 translation);

	/// @brief Add the specified value to this transform's existing local translation
	/// @param translation 
	void addTranslation(glm::vec3 translation);

private:
	glm::mat4 getLocalMatrix();
	glm::mat4 m_localMatrix = glm::mat4(1.0);
	glm::mat4 m_worldMatrix = glm::mat4(1.0);

	glm::vec3 m_translation = glm::vec3(0.f, 0.f, 0.f); //TODO: these don't have a specified range right now
	glm::vec3 m_scale = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_rotation = glm::vec3(0.f, 0.f, 0.f);

	bool m_localDirty = true;
	bool m_worldMatrixDirty = true;
};
#endif