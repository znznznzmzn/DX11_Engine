#include "Framework.h"
#include "Scene_ModelRender.h"

Scene_ModelRender::Scene_ModelRender() {
    model = new Model("PotGirl");
}
Scene_ModelRender::~Scene_ModelRender() {
    SAFE_DELETE(model);
    //delete model;
}

void Scene_ModelRender::Update() {
    //model->Update();
    model->UpdateWorld();
}

void Scene_ModelRender::PreRender() {
}
void Scene_ModelRender::Render() {
    model->Render();
}

void Scene_ModelRender::PostRender() {
}

void Scene_ModelRender::GUIRender() {
    //model->GUIRender();
}
