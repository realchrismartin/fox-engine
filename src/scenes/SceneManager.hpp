#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "src/scenes/Scene.hpp"
#include "src/scenes/SceneLibrary.hpp"

class SceneManager
{
public:
	void loadScene(SceneEnum scene);
	Scene& getCurrentScene() const;
private:
	std::unique_ptr<Scene> m_currentScene;
};

#endif