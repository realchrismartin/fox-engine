#ifndef CONFIGUREDENTITY_HPP
#define CONFIGUREDENTITY_HPP

#include "src/entities/GameEntity.hpp"
#include "src/scenes/SceneConfig.hpp"
#include "src/scenes/Scene.hpp"

struct ConfiguredEntity
{
	size_t entityIndex = 0;
	SceneConfig* config = nullptr;

	void addInitFn(std::function<void(const GameEntity&,Scene&)> initFn)
	{
		if (config == nullptr)
		{
			return;
		}

		config->addInitFnForEntity(entityIndex, initFn);
	}
};

#endif
