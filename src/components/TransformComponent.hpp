#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

class InputComponent;

class TransformComponent
{
public:
	const glm::mat4& getModelMatrix();
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition() const;
private:
	glm::vec3 m_position = glm::vec3(0.f, 0.f, 0.f); //TODO: these don't have a specified range right now
	glm::vec3 m_scale = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_rotation = glm::vec3(0.f, 0.f, 0.f);
};
#endif