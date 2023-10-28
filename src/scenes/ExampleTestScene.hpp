#ifndef EXAMPLETESTSCENE_HPP
#define EXAMPLETESTSCENE_HPP

#include "src/scenes/Scene.hpp"

/// @brief An example scene that has a few entities with components in it for demo purposes.
class ExampleTestScene : public Scene
{
public:
	ExampleTestScene();
protected:
	void createPlayer();
	void createFloor();
	void createObstacles();
private:
	std::optional<int> m_playerUID;
	std::optional<int> m_obstacleUID;
	std::optional<int> m_floorUID;
};

#endif