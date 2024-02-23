#include "Framework.h"
#include "Scene_Terrain.h"

Scene_Terrain::Scene_Terrain() {
	CAMERA->is_freemode = true;

	terrain = new Terrain(L"Data/Maps/Map_01.map");

	robot = new Robot();
	robot->Position() = { 20.0f, 0.0f, 20.0f };
	robot->Position().y = terrain->getHeight(robot->Position().x, robot->Position().z) + 2.0f;
}

Scene_Terrain::~Scene_Terrain() {
	delete terrain;
}

void Scene_Terrain::Update() {
	robot->Position().y = terrain->getHeight(robot->Position().x, robot->Position().z) + 2.0f;
	robot->UpdateWorld();
}

void Scene_Terrain::PreRender() {
}

void Scene_Terrain::Render() {
	terrain->Render();
	robot->Render();
}

void Scene_Terrain::PostRender() {
}

void Scene_Terrain::GUIRender() {
	terrain->GUIRender();
}
