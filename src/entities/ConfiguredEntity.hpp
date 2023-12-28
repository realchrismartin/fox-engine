#ifndef CONFIGUREDENTITY_HPP
#define CONFIGUREDENTITY_HPP

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
