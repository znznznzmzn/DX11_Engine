#include "Framework.h"

FPS_Runner::FPS_Runner() {
	Tag() = "Runner";

	walk_speed = 2.5f;
	dash_speed = 4.2f;

	stat.MaxSP = 30.0f;
	stat.SP = 30.0f;

	HP_ico = new GUI_Basic(L"Textures/GUI/Ico_Energy.png");
	HP_ico->Color().w = 0.9f;
	HP_ico->Scale() = { 32.0f, 32.0f, 0.0f };
	HP_ico->Position() = { 30.0f, 30.0f, 0.0f };
	HP_ico->UpdateWorld();

	HP_Bar = new GUI_ProgressBar(L"Textures/GUI/Basic.png", L"Textures/GUI/Basic_Black.png");
	HP_Bar->Color().w = 0.9f;
	HP_Bar->Color_BG().w = 0.5f;
	HP_Bar->Scale() = { 150.0f, 30.0f, 0.0f };
	HP_Bar->Position() = { HP_Bar->Scale().x / 2.0f + 60.0f, 30.0f, 0.0f };
	HP_Bar->SetValue(stat.SP / stat.MaxSP);

	BodyCollider = new SphereCollider();
	BodyCollider->SetParent(this);
	BodyCollider->Position().y = 1.0f;
	BodyCollider->Scale() = Vector3::one() * 0.5f;
	BodyCollider->UpdateWorld();

	Lantern = new PointLight();
	Lantern->SetParent(this);
	Lantern->Position() = { 0.0f, 2.2f, 0.0f };
	Lantern->Scale() = Vector3::one() * 7.0f;
	Lantern->OnGizmo(false);
	Lantern->UpdateWorld();
}
FPS_Runner::~FPS_Runner() {
	SAFE_DELETE(HP_Bar);
	SAFE_DELETE(HP_ico);

	Lantern->NullParent();
	SAFE_DELETE(Lantern);
}

void FPS_Runner::Move() {
	if (ImGui::GetIO().WantCaptureKeyboard) return; // 에디터 키보드 입력방지
	move_direction = { 0.0f, 0.0f, 0.0f };
	if (KEY_PRESS(ImGuiKey_W)) move_direction += Forward();
	if (KEY_PRESS(ImGuiKey_S)) move_direction -= Forward();
	if (KEY_PRESS(ImGuiKey_D)) move_direction += Right();
	if (KEY_PRESS(ImGuiKey_A)) move_direction -= Right();
	
	 
	if (KEY_PRESS(ImGuiKey_LeftShift)) {
		if (stat.SP > 0.0f) {
			move_speed = dash_speed;
			stat.SP = Calc::Clamp(stat.SP - 9.0f * DELTA, 0.0f, stat.MaxSP);
		}
		else move_speed = walk_speed;
	}
	else {
		move_speed = walk_speed;
		stat.SP = Calc::Clamp(stat.SP + 6.0f * DELTA, 0.0f, stat.MaxSP);
	}
	HP_Bar->SetValue(stat.SP / stat.MaxSP);
	
	Position() += move_direction.normalize() * move_speed * DELTA;

	// 카메라 흔들림
	if (move_direction.x != 0.0f && move_direction.z != 0.0f) { // WSAD로 움직이는중이라면
		walk_time += DELTA * move_speed * walk_vibe;
		walk_damp.x = sin(walk_time);
		walk_damp.y = walk_damp.x * walk_damp.x - 0.1f;
		walk_damp *= walk_dist;
		FirstCam->Position() = Vector3::lerp(FirstCam->Position(), Camera_Offset + walk_damp, DELTA / 0.2f);
	}
	else {
		FirstCam->Position() = Vector3::lerp(FirstCam->Position(), Camera_Offset, DELTA / 0.2f);
		walk_time = 0.0f;
	}
}
void FPS_Runner::Rotate() {
	if (ImGui::GetIO().WantCaptureMouse) return; // 에디터 창에선 마우스 작동 방지 
	Rotation().y += ImGui::GetIO().MouseDelta.x * rotation_speed * DELTA; // y축 회전은 player 자체를 회전
	Rotation().y = Calc::RepeatPI(Rotation().y);
	FirstCam->Rotation().x = Calc::Clamp(
		FirstCam->Rotation().x + ImGui::GetIO().MouseDelta.y * rotation_speed * DELTA, // x축 회전은 카메라
		-Calc::PI / 2.0f, Calc::PI / 2.0f);// x회전 Clamp
}

void FPS_Runner::Update() {
	if (CAMERA->Active()) return; // 메인 카메라 활성화시 작동 X
	Move();
	Rotate();
}
void FPS_Runner::Render() { Lantern->Render(); }
void FPS_Runner::UpdateWorld() {
	C_FirstPerson::UpdateWorld();
	Lantern->UpdateWorld();
	BodyCollider->UpdateWorld();
}

void FPS_Runner::PostRender() { // Game GUI
	if (CAMERA->Active()) return; // 메인 카메라 활성화시 렌더 X
	HP_ico->PostRender();
	HP_Bar->PostRender();
}
void FPS_Runner::GUIRender() {
	Transform::GUIRender();
	if (ImGui::SliderFloat("HP", &stat.HP, 0.0f, stat.MaxHP))
		HP_Bar->SetValue(stat.HP / stat.MaxHP);
}
