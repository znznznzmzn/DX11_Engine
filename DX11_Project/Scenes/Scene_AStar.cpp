#include "Framework.h"
#include "Scene_AStar.h"

Scene_AStar::Scene_AStar() {
	CAMERA->is_freemode = true;

	t = new Terrain(L"Data/Maps/Map_01.map");

	r = new Robot();
	r->setTerrain(t);
	r->Position() = { 15.0f, 2.0f, 15.0f };

	aStar = new AStar();
	aStar->CreateNode(t);
}
Scene_AStar::~Scene_AStar() {
	SAFE_DELETE(t);
	SAFE_DELETE(r);
	SAFE_DELETE(aStar);
}

void Scene_AStar::Update() {
	aStar->Update();
	r->Move();
	r->Animate();
	if (MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse) {
		Vector3 destPos = t->Picking();
		if (!aStar->GetNode(destPos)->IsObstacle()) {
			r->path.clear();
			r->path.push_back(destPos);
			aStar->GetPath(
				aStar->FindCloseNode(r->Position()),
				aStar->FindCloseNode(destPos),
				r->path);
			if (r->path.size() == 1) r->path.clear();
		}
	}
	r->UpdateWorld();
}
void Scene_AStar::PreRender() {
}
void Scene_AStar::Render() {
	t->Render();
	r->Render();
	aStar->Render();
}
void Scene_AStar::PostRender() {
}
void Scene_AStar::GUIRender() {
	aStar->GUIRender();
	t->GUIRender();
}
