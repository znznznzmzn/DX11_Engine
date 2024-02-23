#include "Framework.h"
#include "Scene_Particle.h"

Scene_Particle::Scene_Particle() {
	collider = new BoxCollider();
	collider->Scale() = { 100, 100, 1 };
	collider->UpdateWorld();

	particle = new Rain();
}
Scene_Particle::~Scene_Particle() {
	SAFE_DELETE(collider);
	SAFE_DELETE(particle);
}

void Scene_Particle::Update() {
	if (MOUSE_CLICK(0)) {
		Contact ctt;
		Ray ray = CAMERA->ScreenPointToRay(mousePos);

		if (collider->RayCollision(ray, &ctt)) particle->Play(ctt.hitPoint);
	}
	particle->Update();
}

void Scene_Particle::PreRender(){
}
void Scene_Particle::Render() {
	particle->Render();
	collider->Render();
}
void Scene_Particle::PostRender() {
}

void Scene_Particle::GUIRender() {
	particle->GUIRender();
}
