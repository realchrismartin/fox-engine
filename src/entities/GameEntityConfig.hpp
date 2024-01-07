#ifndef GAMEENTITYCONFIG_HPP
#define GAMEENTITYCONFIG_HPP

#include "src/entities/GameEntity.hpp"
#include "src/scenes/Scene.hpp"
#include "src/components/ComponentTypes.hpp"

struct GameEntityConfig
{
	/// @brief Specify a setup function that will run on the scene when this entity is built.
	/// @param fn 
	/// @return 
	GameEntityConfig whenInit(const std::function<void(const GameEntity&, Scene&)>& fn)
	{
		m_initFn = fn;
		return *this;
	}
	
	/// @brief Run the setup function on the scene.
	/// @param entity 
	/// @param scene 
	void init(const GameEntity& entity, Scene& scene) const
	{
		m_initFn(entity, scene);
	}

private:
	std::function<void(const GameEntity&,Scene&)> m_initFn = [](auto const& entity, auto& scene) {};
};

#endif
