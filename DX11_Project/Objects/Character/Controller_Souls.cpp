#include "Framework.h"

Controller_Souls::Controller_Souls() {
	Tag() = "Soul";

	model = new C_PotMan();
	model->SetParent(this);
	soulCam = new Camera();
	soulCam->is_freemode = false;
	soulCam->Tag() = "SoulCamera";
	soulCam->Position() = Position();

	Root = new Transform();
	Root->SetParent(this);
	camPos = new Transform();
	camPos->Tag() = "camPos";
	camPos->SetParent(Root);
	camPos->Position() = { 0.0f, 1.5f, 2.5f };

	Targeter = new Sphere();
	Targeter->Tag() = "targeter";
	target = nullptr;

	stat.MaxHP = 100.0f;
	stat.HP = 100.0f;
	stat.MaxSP = 80.0f;
	stat.SP = 80.0f;
	
	HP_Bar = new GUI_ProgressBar(L"Textures/GUI/Basic_Red.png", L"Textures/GUI/Basic_Black.png");
	HP_Bar->Scale() = { 300.0f, 20.0f, 0.0f };
	HP_Bar->Position() = { HP_Bar->Scale().x / 2.0f + 10.0f, SCREEN_HEIGHT - 20.0f, 0.0f };
	HP_Bar->SetValue(stat.HP / stat.MaxHP);

	SP_Bar = new GUI_ProgressBar(L"Textures/GUI/Basic_Green.png", L"Textures/GUI/Basic_Black.png");
	SP_Bar->Scale() = { 240.0f, 20.0f, 0.0f };
	SP_Bar->Position() = { SP_Bar->Scale().x / 2.0f + 10.0f, SCREEN_HEIGHT - 50.0f, 0.0f };
	SP_Bar->SetValue(stat.SP / stat.MaxSP);
}
Controller_Souls::~Controller_Souls() {
	SAFE_DELETE(soulCam);
	SAFE_DELETE(HP_Bar);
	SAFE_DELETE(SP_Bar);
}

void Controller_Souls::Control() {
	if (CAMERA->Active()) return;

	{ // CAMERA
		// 카메라 위치
		soulCam->Position() = Vector3::lerp(soulCam->Position(), camPos->GlobalPosition(), DELTA / 0.1f);

		// 타게팅
		if (target != nullptr) { // 타게팅이 온일 경우
			// soulCam->LookAt(target->Position());
			Root->Rotation().y = atan2f(
				Position().x - target->Position().x,
				Position().z - target->Position().z);

			soulCam->Rotation().y = atan2f( 
				Position().x - soulCam->Position().x,
				Position().z - soulCam->Position().z);
		}
		else { // target_Default일때는 자유로운 카메라 회전 가능
			soulCam->Rotation().y = atan2f( // 회전은 Root의 회전, 즉 카메라 y회전값을 고정해도 상관X
				Position().x - soulCam->Position().x,
				Position().z - soulCam->Position().z);
		}

		// x축 회전
		soulCam->Rotation().x += ImGui::GetIO().MouseDelta.y * rotationSpeed * DELTA; // x축 회전은 카메라
		soulCam->Rotation().x = Calc::Clamp(soulCam->Rotation().x, -XM_PI / 4.0f, XM_PI / 5.0f);

		camPos->Position().y = 1.2f + 2.0f * soulCam->Rotation().x; // 회전각에 따라 높낮이 조정
		camPos->Position().y = Calc::Clamp(camPos->Position().y, 0.0f, 20.0f); // 조정 최대 최소 갈무리

		Root->Rotation().y += ImGui::GetIO().MouseDelta.x * rotationSpeed * DELTA; // 기본 마우스 y축 회전
		Root->Rotation().y = Calc::RepeatPI(Root->Rotation().y); // Repeat
	}

	// 이동
	if (t != nullptr) height = t->getHeight(Position().x, Position().z);
	Position().y = Calc::Lerp(Position().y, height, 0.1f);

	if (model->isStep()) {
		if (move_dir.x == 0 && move_dir.z == 0) move_dir = model->Forward();
		moveSpeed = stepSpeed;
		Position() += move_dir * moveSpeed * DELTA;
		return;
	}

	if (MOUSE_CLICK(0)) { model->animate_Attack(); return; }

	move_dir = Vector3::zero(); // 이동방향 초기화 (normalize()를 하기 때문에 0으로 초기화)

	if (KEY_PRESS(ImGuiKey_Space)) moveSpeed = Calc::Lerp(moveSpeed, sprintSpeed, DELTA); // 이속 증가
	else moveSpeed = Calc::Lerp(moveSpeed, walkSpeed, DELTA); // 이속 회복

	if (KEY_PRESS(ImGuiKey_W)) move_dir += soulCam->Forward();
	if (KEY_PRESS(ImGuiKey_S)) move_dir -= soulCam->Forward();
	if (KEY_PRESS(ImGuiKey_A)) move_dir -= soulCam->Right  ();
	if (KEY_PRESS(ImGuiKey_D)) move_dir += soulCam->Right  ();
	move_dir = move_dir.normalize(); // 이동방향 일반화 (대각선 키입력 보간)

	if (move_dir.x != 0.0f || move_dir.z != 0.0f) { // 움직일때 : 모델이 회전방향으로 부드럽게 회전
		moveRotation = atan2f(-move_dir.x, -move_dir.z);

		model_rot = (target != nullptr) ? // 타겟이 있으면
			((moveSpeed < stepSpeed) ?
				Root->Rotation().y : // 타겟 방향으로
				moveRotation) : // 그러나 이동속도가 높으면 이동방향으로
			moveRotation; // 타겟이 없으면 이동 방향으로
		model_rot += (model->Rotation().y < 0) ? // 회전방향 초과분 검사
			((model_rot - model->Rotation().y > XM_PI) ? -XM_2PI : 0.0f) :
			((model->Rotation().y - model_rot > XM_PI) ? +XM_2PI : 0.0f);
		model->Rotation().y = Calc::Lerp(model->Rotation().y, model_rot, DELTA / 0.3f); // 적용
		model->Rotation().y = Calc::Repeat(model->Rotation().y, -XM_PI, XM_PI); // 초과분 Repeat
	}
	else moveSpeed = 1.99f;

	if (model->isAttack()) { return; } // 공격중일경우 탈출

	if (KEY_UP(ImGuiKey_Space)) {
		if (moveSpeed < stepSpeed) { // 이동속도가 어느정도 정상화 됐을때
			model->animate_Step(moveRotation);
			SP_Bar->SetValue((stat.SP -= 10.0f) / stat.MaxSP);
			return;
		}
	}
	Position() += move_dir * moveSpeed * DELTA; // 이동 적용

	model->animate_Move(moveRotation, moveSpeed); // 이동 방향에 따라 이동 애니메이션 작동
}

void Controller_Souls::PreRender() { soulCam->PreRender(); }
void Controller_Souls::Update() {
	model->Update();

	// Targeting
	if (target != nullptr) {
		Targeter->Position() = target->GlobalPosition();
		Targeter->UpdateWorld();
	}

	// Camera
	Root->UpdateWorld();
	camPos->UpdateWorld();
	soulCam->UpdateWorld();
	this->UpdateWorld();
}
void Controller_Souls::Render() { model->Render(); }

void Controller_Souls::PostRender() { // GUI
	HP_Bar->PostRender();
	SP_Bar->PostRender();
}

void Controller_Souls::GUIRender() {
	model->GUIRender();
	camPos->GUIRender();
	soulCam->GUIRender();
}

void Controller_Souls::SetTarget(Transform* target) { 
	if (this->target == target) this->target = nullptr;
	else this->target = target;
}
