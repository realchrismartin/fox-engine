#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

class InputComponent;

class TransformComponent
{
public:
	glm::mat4 getModelMatrix();
	void setModelMatrix(const glm::mat4& matrix);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition() const;
	const std::set<int>& getChildren() const
	{
		return m_children;
	}
	void addChild(int child)
	{
		m_children.insert(child);
	}
	void removeChild(int child)
	{
		m_children.erase(child);
	}
private:
	glm::mat4 m_matrix = glm::mat4(1.0); //TODO: stop resetting this constantly?
	glm::vec3 m_position = glm::vec3(0.f, 0.f, 0.f); //TODO: these don't have a specified range right now
	glm::vec3 m_scale = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_rotation = glm::vec3(0.f, 0.f, 0.f);
	std::set<int> m_children;
};
#endif