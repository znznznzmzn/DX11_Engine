#include "Framework.h"
#include "Scene_Grid.h"

Scene_Grid::Scene_Grid() {
	grid = new Grid();
}

Scene_Grid::~Scene_Grid() {
	SAFE_DELETE(grid);
}

void Scene_Grid::Update() {
	Calc::Easing::Pow2(5);
}

void Scene_Grid::PreRender() {
}

void Scene_Grid::Render() {
	grid->Render();
}

void Scene_Grid::PostRender() {
}

void Scene_Grid::GUIRender() {
}
