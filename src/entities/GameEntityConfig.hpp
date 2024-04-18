#ifndef GAMEENTITYCONFIG_HPP
#define GAMEENTITYCONFIG_HPP

class Scene;

struct GameEntityConfig
{
	GameEntityConfig whenInit(const std::function<void(int, Scene&)>& fn);
	GameEntityConfig startsActive(bool state);
	void init(int entityUID, Scene& scene) const;
	bool getDefaultActiveState() const;
private:
	std::function<void(int,Scene&)> m_initFn = [](int entityUID, auto& scene) {};
	bool m_active = true; //Whether the entity is active by default
};

#endif
