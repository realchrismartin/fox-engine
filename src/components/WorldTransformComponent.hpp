#ifndef WORLDTRANSFORMCOMPONENT_HPP
#define WORLDTRANSFORMCOMPONENT_HPP

class WorldTransformComponent
{
public:
	void setWorldMatrix(const glm::mat4& worldMatrix); 
	const glm::mat4& getWorldMatrix() const;
private:
	glm::mat4 m_worldMatrix = glm::mat4(1.0);
};
#endif