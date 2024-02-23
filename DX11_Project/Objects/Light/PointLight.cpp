#include "Framework.h"


void PointLight::UpdateData() {
	plBuffer->data.pointPosition = GlobalPosition();
	plBuffer->data.pointRange = max(GlobalScale().x, max(GlobalScale().y, GlobalScale().z));
}

PointLight::PointLight() {
	Tag() = "PointLight";

	world = XMMatrixIdentity();
	wBuffer = new MatrixBuffer();
	plBuffer = new PointLightBuffer();

	Bulb = new GUI_Basic(L"Textures/GUI/Ico_Default.png");
	Bulb->Scale() = Vector3::one() * 32.0f;
}
PointLight::~PointLight() {
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(plBuffer);
	SAFE_DELETE(Bulb);
}

void PointLight::Set() {
	if (!Active()) return;
	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	plBuffer->SetPSBuffer(2);
}

void PointLight::UpdateWorld() {
	if (IsGizmo) {
		Bulb->Position() = CAMERA->WorldToScreenPoint(Position());
		Bulb->UpdateWorld();
	}
	Transform::UpdateWorld();
	UpdateData();
}

void PointLight::GUIRender() {
	Transform::GUIRender();
	ImGui::ColorEdit3((tag + "Color").c_str(), (float*)&plBuffer->data.pointColor);
}


