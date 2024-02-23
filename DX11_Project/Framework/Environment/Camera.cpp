#include "Framework.h"

void Camera::FreeMode() {
	if (is_freemode) {
		if (KEY_UP(ImGuiKey_F1)) { // FreeMode 종료 감지
			is_freemode = false;
			return;
		}
		if (MOUSE_PRESS(1)) { // 우클릭
			// 마우스 회전
			Rotation().x += ImGui::GetIO().MouseDelta.y * rotation_speed * DELTA;
			Rotation().y += ImGui::GetIO().MouseDelta.x * rotation_speed * DELTA;
			Rotation().x = Calc::Clamp(Rotation().x, -Calc::PI / 2.0f, Calc::PI / 2.0f);

			// 키보드 이동
			direction = { 0.0f, 0.0f, 0.0f };
			move_speed = (KEY_PRESS(ImGuiKey_LeftShift)) ? move_speed_dash : move_speed_normal;
			if (KEY_PRESS(ImGuiKey_W)) direction += Forward();
			if (KEY_PRESS(ImGuiKey_S)) direction -= Forward();
			if (KEY_PRESS(ImGuiKey_D)) direction += Right();
			if (KEY_PRESS(ImGuiKey_A)) direction -= Right();
			if (KEY_PRESS(ImGuiKey_E)) direction += Up();
			if (KEY_PRESS(ImGuiKey_Q)) direction -= Up();
			direction.normalize();
			Position() += direction * move_speed * DELTA;
		}
	}
	else {
		if (KEY_UP(ImGuiKey_F1)) is_freemode = true; // FreeMode 시작 감지
	}
	UpdateWorld();
}
void Camera::UpdateWorld() {
	Transform::UpdateWorld();
	invView = world;
	view = XMMatrixInverse(nullptr, world);
	viewBuffer->Set(view, world);
}

void Camera::Update() {
	if (!Active()) return;
	FreeMode();
}
void Camera::GUIRender() {
	ImGui::Text("Cam_IsFreeMode : %s", ((is_freemode)? "On" : "Off"));
	if (ImGui::TreeNode("CameraOption")) {
		ImGui::Indent();
		ImGui::InputFloat3("CamPos", (float*)&Position(), "%.2f");
		ImGui::InputFloat3("CamRot", (float*)&Rotation(), "%.2f");
		ImGui::DragFloat("RotSpeed", &rotation_speed);
		ImGui::DragFloat("MoveSpeed", &move_speed_normal);
		ImGui::DragFloat("Move_Dash", &move_speed_dash);
		ImGui::DragFloat("WheelTrace", &ImGui::GetIO().MouseWheel);
		ImGui::Unindent();
		ImGui::TreePop();
	}
}

void Camera::SetViewBuffer() { 
	if (Active()) {
		viewBuffer->SetVSBuffer(1);
		viewBuffer->SetGSBuffer(1);
	}
}

Ray Camera::ScreenPointToRay(Vector3 screenPos) {
	float2 point;
	point.x = (2.0f * (screenPos.x / (float)SCREEN_WIDTH )) - 1.0f; // -1~1 스크린 좌표 -> NDC좌표
	point.y = (2.0f * (screenPos.y / (float)SCREEN_HEIGHT)) - 1.0f;
	float4x4 tmp;
	XMStoreFloat4x4(&tmp, Environment::Get()->GetProjection());
	point.x /= tmp._11;
	point.y /= tmp._22;
	return Ray{ Position(), (Vector3(point.x, point.y, 1.0f) * invView).normalize() };
}

Vector3 Camera::WorldToScreenPoint(Vector3 worldPos) {
	Vector3 screenPos;
	screenPos = XMVector3TransformCoord(worldPos, view);
	screenPos = XMVector3TransformCoord(screenPos, Environment::Get()->GetProjection());
	screenPos.x = SCREEN_WIDTH  * (screenPos.x + 1.0f) * 0.5f; // NDC좌표 = -1 ~ 1 >> 변환 >> 0 ~ SCREEN_WIDTH
	screenPos.y = SCREEN_HEIGHT * (screenPos.y + 1.0f) * 0.5f; 
	return screenPos;
}

void Camera::LookAt(Vector3 target, Vector3 upVector) {	
	view = XMMatrixLookAtLH(Position(), target, upVector);
	viewBuffer->Set(view, XMMatrixInverse(nullptr, view));
}

Camera::Camera() {
	viewBuffer = new ViewBuffer();
	view = XMMatrixIdentity();
	invView = XMMatrixIdentity();
}
Camera::~Camera() { SAFE_DELETE(viewBuffer); }
