#include "Framework.h"

void SpotLight::UpdateData() {
	slBuffer->data.spotPosition = GlobalPosition();
	slBuffer->data.spotRange = GlobalScale().z;
	slBuffer->data.spotDirection = GlobalRotation();
	slBuffer->data.spotRadius = max(GlobalScale().x, GlobalScale().y);
}

SpotLight::SpotLight() {
	Tag() = "SpotLight";

	world = XMMatrixIdentity();
	wBuffer = new MatrixBuffer();
	slBuffer = new SpotLightBuffer();

	Bulb = new GUI_Basic(L"Textures/GUI/Ico_Default.png");
	Bulb->Scale() = Vector3::one() * 32.0f;
}
SpotLight::~SpotLight() {
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(slBuffer);
	SAFE_DELETE(Bulb);
}

void SpotLight::Set() {
	if (!Active()) return;
	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	slBuffer->SetPSBuffer(3);
}
void SpotLight::UpdateWorld() {
	if (IsGizmo) {
		Bulb->Position() = CAMERA->WorldToScreenPoint(Position());
		Bulb->UpdateWorld();
	}
	Transform::UpdateWorld();
	UpdateData();
}
void SpotLight::GUIRender() {
	Transform::GUIRender();
	ImGui::ColorEdit3((tag + "Color").c_str(), (float*)&slBuffer->data.spotColor);
}
