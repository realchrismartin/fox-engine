 #include "src/entities/GameEntityConfig.hpp"

#include "src/entities/GameEntity.hpp"
#include "src/scenes/Scene.hpp"

	GameEntityConfig GameEntityConfig::whenInit(const std::function<void(int, Scene&)>& fn)
	{
		m_initFn = fn;
		return *this;
	}

	GameEntityConfig GameEntityConfig::startsActive(bool state)
	{
		m_active = state;
		return *this;
	}
	
	void GameEntityConfig::init(int entityUID, Scene& scene) const
	{
		m_initFn(entityUID, scene);
	}

	bool GameEntityConfig::getDefaultActiveState() const
	{
		return m_active;
	}