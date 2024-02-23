#include "Framework.h"

C_FirstPerson::C_FirstPerson() {
    FirstCam = new Camera();
    FirstCam->Tag() = "FirstPersonCamera";
    FirstCam->SetParent(this);
    FirstCam->Position() = Camera_Offset;
}
C_FirstPerson::~C_FirstPerson() { }

void C_FirstPerson::Move() { // move
	if (t != nullptr) Position().y = t->getHeight(Position());
	move_direction = { 0.0f, 0.0f, 0.0f };
	if (KEY_PRESS(ImGuiKey_W)) move_direction += Forward();
	if (KEY_PRESS(ImGuiKey_S)) move_direction -= Forward();
	if (KEY_PRESS(ImGuiKey_D)) move_direction += Right();
	if (KEY_PRESS(ImGuiKey_A)) move_direction -= Right();
	move_speed = (KEY_PRESS(ImGuiKey_LeftShift)) ? dash_speed : walk_speed;
	Position() += move_direction.normalize() * move_speed * DELTA;
}

void C_FirstPerson::Rotate() { // rotation
	Rotation().y += ImGui::GetIO().MouseDelta.x * rotation_speed * DELTA;
	FirstCam->Rotation().x += ImGui::GetIO().MouseDelta.y * rotation_speed * DELTA; // x축 회전은 카메라
	FirstCam->Rotation().x = Calc::Clamp(FirstCam->Rotation().x, -Calc::PI / 2.0f, Calc::PI / 2.0f);
}
void C_FirstPerson::UpdateWorld() {
	Transform::UpdateWorld();
	FirstCam->UpdateWorld();
}
void C_FirstPerson::Update() { // Basic_Control + UpdateWorld
	Move();
	Rotate(); 
}
void C_FirstPerson::PreRender() {  // Camera_Render
	if (CAMERA->Active()) return; // 메인 카메라 활성화시 랜더 X
	FirstCam->PreRender(); 
}
void C_FirstPerson::GUIRender() {
	ImGui::DragFloat("WalkSpeed", &walk_speed);
	ImGui::DragFloat("DashSpeed", &dash_speed);
	ImGui::DragFloat("RotSpeed", &rotation_speed);
}

