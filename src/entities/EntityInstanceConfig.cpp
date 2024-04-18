 #include "src/entities/EntityInstanceConfig.hpp"

#include "src/scenes/SceneConfig.hpp"

void EntityInstanceConfig::addInitFn(std::function<void(int,Scene&)> initFn)
{
    if (sceneConfig == nullptr)
    {
        return;
    }

    sceneConfig->addInitFnForEntity(*this, initFn);
}

void EntityInstanceConfig::addChild(const EntityInstanceConfig& childEntity)
{
    if (sceneConfig == nullptr)
    {
        return;
    }

    sceneConfig->addChild(*this, childEntity);
}