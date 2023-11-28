#include "src/scenes/ExampleTestScene.hpp"

#include "src/entities/GameEntity.hpp"

#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/graphics/ModelConfig.hpp"
#include <format>

ExampleTestScene::ExampleTestScene()
{
	createPlayer();
	createFloor();
	createWindmill();
	createBush();
}

void ExampleTestScene::createPlayer()
{
	if (m_playerUID.has_value())
	{
		removeEntity(m_playerUID.value());
	}

	m_playerUID = createEntity();

	if (m_playerUID.has_value())
	{
		int uid = m_playerUID.value();

		addComponent<InputComponent>(uid);

		ModelConfig model;
		model.spriteSize = { 1024.f,1024.f };
		model.spriteOffsetOnTexture = { 1023.f,0 };

		int m = 0;
		for (int i = 0; i < 60; i++)
		{
			MeshConfig mesh;
			mesh.meshFilePath = std::format("../../img/mushroom/mushroom{}.obj", m);
			model.meshes.push_back(mesh);
			if (i % 3 == 0)
			{
				m++;
			}
		}

		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setScale({ 2.f,2.f,2.f});
		setCameraTargetEntity(uid);
	}

}

void ExampleTestScene::createFloor()
{
	if (m_floorUID.has_value())
	{
		removeEntity(m_floorUID.value());
	}

	m_floorUID = createEntity();

	if (m_floorUID.has_value()) 
	{
		int uid = m_floorUID.value();

		ModelConfig model;
		model.spriteSize = { 512,512 };
		model.spriteOffsetOnTexture = { 511,0 };

		MeshConfig mesh;
		mesh.meshFilePath = "../../img/cube.obj";

		model.meshes.push_back(mesh);

		loadModel(model, uid);

		getComponent<TransformComponent>(uid).setScale({ 100.f,1.f,100.f });
		getComponent<TransformComponent>(uid).setTranslation({ 0.f,-2.f,0.f});
	}

}

void ExampleTestScene::createWindmill()
{
	if (m_windmillUID.has_value())
	{
		removeEntity(m_windmillUID.value());
	}

	m_windmillUID = createEntity();

	if (m_windmillUID.has_value()) 
	{
		int uid = m_windmillUID.value();
		MeshConfig mesh;
		ModelConfig model;
		model.meshes.push_back(mesh);

		loadModel(model, uid);

		getComponent<TransformComponent>(uid).setRotation({ 0.f,180.f,0.f});
	}
}

void ExampleTestScene::createBush()
{
	m_bushUID = createEntity();

	if (m_bushUID.has_value())
	{
		int uid = m_bushUID.value();

		ModelConfig model;
		model.spriteSize = { 228,228 };
		model.spriteOffsetOnTexture = { 0,513 };
		MeshConfig mesh;
		mesh.meshFilePath = "../../img/quoteunquote-bush.obj";
		model.meshes.push_back(mesh);

		loadModel(model, uid);

		getComponent<TransformComponent>(uid).setTranslation({ 10.f,0.f,5.f});
		addChild(m_windmillUID.value(), uid);
	}
}
