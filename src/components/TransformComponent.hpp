#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

class InputComponent;

class TransformComponent
{
public:
	glm::mat4& getWorldMatrix();

	void updateWorldMatrix(const glm::mat4& parentWorldMatrix);

	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setTranslation(glm::vec3 position);
private:
	void updateLocalMatrix();

	glm::mat4 m_localMatrix = glm::mat4(1.0);
	glm::mat4 m_worldMatrix = glm::mat4(1.0);

	glm::vec3 m_translation = glm::vec3(0.f, 0.f, 0.f); //TODO: these don't have a specified range right now
	glm::vec3 m_scale = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_rotation = glm::vec3(0.f, 0.f, 0.f);

	bool m_dirty = true;
};
#endif