#include "src/scenes/SceneManager.hpp"

void SceneManager::loadScene(SceneEnum scene)
{
	//Tear down the existing scene, if there is one
	if (m_currentScene)
	{
		//Call all of the component pool destructors and whatnot.
		m_currentScene.reset();
	}

	//Allocate the new scene
	m_currentScene = std::make_unique<Scene>(scene);
}

Scene& SceneManager::getCurrentScene() const
{
	return *m_currentScene;
}
