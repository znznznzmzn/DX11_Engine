#include "Framework.h"
#include "Scene_Collision.h"

Scene_Collision::Scene_Collision() {
	Collider* collider = new CapsuleCollider();
	collider->Tag() = "Capsule1";
	collider->UpdateWorld();
	colliders.push_back(collider);

	collider = new SphereCollider();
	collider->Tag() = "Sphere1";
	colliders.push_back(collider);
}
Scene_Collision::~Scene_Collision() {
	for (Collider* elem : colliders) { SAFE_DELETE(elem); }
	colliders.clear();
}

void Scene_Collision::Update() {
	Ray ray = CAMERA->ScreenPointToRay(mousePos);
	Contact contact;

	if (colliders[0]->RayCollision(ray, &contact)) {
	    colliders[0]->SetColor(1, 0, 0);
	    colliders[1]->Position() = contact.hitPoint;
	}
	else {
	    colliders[0]->SetColor(0, 1, 0);
	}

	for (Collider* collider : colliders)
		collider->UpdateWorld();
}

void Scene_Collision::PreRender() {
}

void Scene_Collision::Render() {
	for (Collider* elem : colliders) elem->Render();
}

void Scene_Collision::PostRender() {
}

void Scene_Collision::GUIRender() {
	for (Collider* elem : colliders) elem->GUIRender();
}
