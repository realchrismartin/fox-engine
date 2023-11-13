#ifndef MVPTRANSFORMCOMPONENT_HPP
#define MVPTRANSFORMCOMPONENT_HPP

class MVPTransformComponent
{
public:
	void setMVPMatrix(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	const glm::mat4& getMVPMatrix() const;
private:
	glm::mat4 m_mvpMatrix = glm::mat4(1.0);
};
#endif