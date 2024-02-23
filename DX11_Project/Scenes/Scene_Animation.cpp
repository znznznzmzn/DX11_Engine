#include "Framework.h"
#include "Scene_Animation.h"

Scene_Animation::Scene_Animation() {
	model = new ModelAnimator("ArcherWithBow");
	model->ReadClip("StandingWalkRight");
	model->PlayClip(0);
	model->Load();
}

Scene_Animation::~Scene_Animation() {
	SAFE_DELETE(model);
}

void Scene_Animation::Update() {
	model->UpdateFrame();
	model->UpdateWorld();
}

void Scene_Animation::PreRender() {
}
void Scene_Animation::Render() {
	model->Render();
}
void Scene_Animation::PostRender() {
}

void Scene_Animation::GUIRender() {
	model->GUIRender();
}
