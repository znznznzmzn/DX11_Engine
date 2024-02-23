#include "Framework.h"

#include "Scenes/Scene_Grid.h"
#include "Scenes/Scene_Light.h"
#include "Scenes/Scene_AStar.h"
#include "Scenes/Scene_TerrainEditor.h"
#include "Scenes/Scene_Terrain.h"
#include "Scenes/Scene_ModelExporter.h"
#include "Scenes/Scene_ModelRender.h"
#include "Scenes/Scene_Animation.h"
#include "Scenes/Scene_Collision.h"
#include "Scenes/Scene_Instancing.h"
#include "Scenes/Scene_Billboard.h"
#include "Scenes/Scene_Particle.h"

string projectPath;

GameManager::GameManager() {
	{ // projectPath »ý¼º
		char path[128];
		GetCurrentDirectoryA(128, path);
		projectPath = path;
	}

	Create();
	SCENE->Create("Grid", new Scene_Grid()); SCENE->Add("Grid");
	//SCENE->Create("Export", new Scene_ModelExporter()); SCENE->Add("Export");
	//SCENE->Create("Scene", new Scene_Terrain());
	//SCENE->Create("Scene", new Scene_TerrainEditor());
	//SCENE->Create("Scene", new Scene_Light());
	//SCENE->Create("Scene", new Scene_ModelRender());
	//SCENE->Create("Scene", new Scene_Animation());
	//SCENE->Create("Scene", new Scene_Collision());
	//SCENE->Create("Scene", new Scene_Instancing());
	//SCENE->Create("Scene", new Scene_AStar());
	//SCENE->Create("Scene", new Scene_Billboard());
	//SCENE->Create("Scene", new Scene_Particle());

	SCENE->Add("Scene");
}
GameManager::~GameManager() { Delete(); }

void GameManager::Update() {
	Environment::Get()->Update();
	SCENE->Update();
}

void GameManager::Render() {
	SCENE->PreRender();
	Device::Get()->Clear();
	Environment::Get()->Set();
	SCENE->Render();
	Environment::Get()->PostSet();
	SCENE->PostRender();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	static bool isActive = true;
	if (isActive) {
		ImGui::Begin("Inspector", &isActive); 
		Environment::Get()->GUIRender();
		SCENE->GUIRender();
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Device::Get()->Present();
}

void GameManager::Create() { 
	Device::Get(); 
	Environment::Get();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete() { 
	Device::Delete(); 
	Environment::Delete();
	Texture::Clear();
	Shader::Delete();
	SceneManager::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
