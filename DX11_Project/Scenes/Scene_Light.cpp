#include "Framework.h"
#include "Scene_Light.h"

Scene_Light::Scene_Light() {
	obj_1 = new Cube();
	obj_2 = new Sphere(30, 30);
	obj_3 = new Cylinder();

	obj_1->Position().x -= 1.5f;
	obj_3->Position().x += 1.5f;

	obj_1->GetMaterial()->SetName("obj_1");
	obj_2->GetMaterial()->SetName("obj_2");
	obj_3->GetMaterial()->SetName("obj_3");

	spight = new SpotLight();
}

Scene_Light::~Scene_Light() {
	delete obj_1;
	delete obj_2;
	delete obj_3;
	delete spight;
}

void Scene_Light::Update() {
	if (!CAMERA->is_freemode) {
		obj_1->Rotation().x += DELTA * 1;
		obj_1->Rotation().y += DELTA * 2;
		obj_1->Rotation().z += DELTA * 3;

		obj_2->Rotation().x += DELTA * 2;
		obj_2->Rotation().y += DELTA * 1;
		obj_2->Rotation().z += DELTA * 3;

		obj_3->Rotation().x += DELTA * 3;
		obj_3->Rotation().y += DELTA * 2;
		obj_3->Rotation().z += DELTA * 1;
	}

	obj_1->UpdateWorld();
	obj_2->UpdateWorld();
	obj_3->UpdateWorld();

	spight->UpdateWorld();
}

void Scene_Light::PreRender() {
}

void Scene_Light::Render() {

	obj_1->Render();
	obj_2->Render();
	obj_3->Render();
	spight->Render();
}

void Scene_Light::PostRender() {
	spight->PostRender();
}

void Scene_Light::GUIRender() {
	obj_1->GUIRender();
	obj_2->GUIRender();
	obj_3->GUIRender();
	obj_1->GetMaterial()->GUIRender();
	obj_2->GetMaterial()->GUIRender();
	obj_3->GetMaterial()->GUIRender();

	spight->GUIRender();
}
