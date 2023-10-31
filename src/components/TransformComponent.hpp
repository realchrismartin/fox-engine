#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP


class PhysicsComponent;

class TransformComponent
{
public:
    void updateWithPhysics(const PhysicsComponent& component);
	const glm::mat4& getModelMatrix(); //TODO: dont need this later
private:
	glm::mat4 m_modelMatrix = glm::mat4(1.0);
};
#endif