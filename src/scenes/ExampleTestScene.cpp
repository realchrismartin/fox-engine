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


		for (int i = 0; i < 21; i++)
		{
			MeshConfig mesh;
			mesh.spriteSize = { 1024.f,1024.f };
			mesh.spriteOffsetOnTexture = { 1023.f,0 };
			mesh.meshFilePath = std::format("../../img/mushroom/mushroom{}.obj", i);
			model.meshes.push_back(mesh);
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
		MeshConfig mesh;
		mesh.meshFilePath = "../../img/cube.obj";
		mesh.spriteSize = { 512,512 };
		mesh.spriteOffsetOnTexture = { 511,0 };

		ModelConfig model;
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
		MeshConfig mesh;
		mesh.meshFilePath = "../../img/quoteunquote-bush.obj";
		mesh.spriteSize = { 228,228 };
		mesh.spriteOffsetOnTexture = { 0,513 };
		ModelConfig model;
		model.meshes.push_back(mesh);

		loadModel(model, uid);
		getComponent<TransformComponent>(uid).setTranslation({ 10.f,0.f,5.f});
		addChild(m_windmillUID.value(), uid);
	}
}
