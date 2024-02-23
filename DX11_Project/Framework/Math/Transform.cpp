#include "Framework.h"

Transform:: Transform() : localScale(1.0f, 1.0f, 1.0f) { world = XMMatrixIdentity(); }
Transform::~Transform() { SAFE_DELETE(parent); }

void Transform::UpdateWorld() {
	if (!Active()) return;
	world = XMMatrixTransformation(pivot, XMQuaternionIdentity(),
		localScale, pivot, 
		XMQuaternionRotationRollPitchYawFromVector(localRotation), 
		localPosition);
	if (parent != nullptr) world *= *parent;
	XMMatrixDecompose(globalScale.GetData(), globalRotation.GetData(), globalPosition.GetData(), world);
}

Vector3 Transform::Forward() { return (Vector3(0, 0, 1) * world).normalize(); }
Vector3 Transform::Up()      { return (Vector3(0, 1, 0) * world).normalize(); }
Vector3 Transform::Right()   { return (Vector3(1, 0, 0) * world).normalize(); }

/*
Transform& Transform::LookAt(Vector3& target) {
	Vector3 tmp = (target - localPosition).normalize();
	localRotation = {
		0.0f, //atan2f(tmp.y, tmp.z),
		atan2f(tmp.x, tmp.z),
		0.0f //atan2f(target.x - localPosition.x, target.y - localPosition.y)
	};
	return *this;
}*/

bool Transform::Active() {
	if (parentTransform == nullptr) return is_active;
	if (!is_active) return false;
	return parentTransform->Active();
}

void Transform::GUIRender() {
	if (ImGui::TreeNode((tag + "_Transform").c_str())) {
		ImGui::Checkbox(tag.c_str(), &is_active);
		ImGui::DragFloat3((tag + "_Pos").c_str(), (float*)&localPosition, 0.01f);
		ImGui::DragFloat3((tag + "_Rot").c_str(), (float*)&localRotation, 0.01f);
		ImGui::DragFloat3((tag + "_Scl").c_str(), (float*)&localScale, 0.01f);
		if (ImGui::Button("Save")) Save(); ImGui::SameLine(); if (ImGui::Button("Load")) Load();

		ImGui::TreePop();
	}
}

void Transform::Save() {
	BinaryWriter* w = new BinaryWriter("Data/Transforms/" + tag + ".srt");
	w->Vector(localPosition);
	w->Vector(localRotation);
	w->Vector(localScale);
	delete w;
}

void Transform::Load() {
	BinaryReader* r = new BinaryReader("Data/Transforms/" + tag + ".srt");
	if (r->IsFailed()) return;
	localPosition = r->Vector();
	localRotation = r->Vector();
	localScale    = r->Vector();
	UpdateWorld();
	delete r;
}

void Transform::SetTransformation(Transformation const& value){
	Position() = value.Position;
	Rotation() = value.Rotation;
	Scale()    = value.Scale;
}

void Transform::Lerp(Transform*& to, float const& speed) {
	Lerp(Transformation(to->Position(), to->Rotation(), to->Scale()), speed);
}
void Transform::Lerp(Transformation to, float const& speed) {
	Position() = Vector3::lerp(Position(), to.Position, speed);
	Rotation() = Vector3::lerp(Rotation(), to.Rotation, speed);
	Scale()    = Vector3::lerp(Scale   (), to.Scale   , speed);
}
