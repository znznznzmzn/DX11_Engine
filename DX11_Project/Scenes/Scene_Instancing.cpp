#include "Framework.h"
#include "Scene_Instancing.h"

Scene_Instancing::Scene_Instancing() {
	modelInstancing = new ModelAnimatorInstancing("PotMan", 128);
	modelInstancing->ReadClip("PotMan_Twerk");
	modelInstancing->ReadClip("PotMan_Attack_1");

	for (float z = -2.0f; z < 2.0f; z++) {
		for (float x = -2.0f; x < 2.0f; x++) {
			Transform* transform = modelInstancing->Add();
			transform->Position() = { x, 0.0f, z };
			transform->Scale() *= 0.01f;
		}
	}
}

Scene_Instancing::~Scene_Instancing() {
	SAFE_DELETE(modelInstancing);
}

void Scene_Instancing::Update() {
	modelInstancing->Update();
}
void Scene_Instancing::PreRender() {
}
void Scene_Instancing::Render() {
	modelInstancing->Render();
}
void Scene_Instancing::PostRender() {
}
void Scene_Instancing::GUIRender() {
	ImGui::SliderInt("Idx", &idx, 0, modelInstancing->instanceCount());
	ImGui::SliderInt("clipIdx", &clip_idx, 0, 1);
	if (ImGui::Button("PlayClip")) modelInstancing->PlayClip(idx, clip_idx);

	modelInstancing->GUIRender();
}
