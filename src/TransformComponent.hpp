#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "src/Component.hpp" 
#include "SFML/System/Vector2.hpp"
#include "src/InputComponent.hpp"

class TransformComponent : public Component
{
public:
	TransformComponent();
	virtual void update() override;
	void updateWithInput(InputComponent& inputComponent);
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f position);
private:
	sf::Vector2f m_position = sf::Vector2f(0.f,0.f);
};

#endif