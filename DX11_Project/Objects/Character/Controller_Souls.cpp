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
		// ī�޶� ��ġ
		soulCam->Position() = Vector3::lerp(soulCam->Position(), camPos->GlobalPosition(), DELTA / 0.1f);

		// Ÿ����
		if (target != nullptr) { // Ÿ������ ���� ���
			// soulCam->LookAt(target->Position());
			Root->Rotation().y = atan2f(
				Position().x - target->Position().x,
				Position().z - target->Position().z);

			soulCam->Rotation().y = atan2f( 
				Position().x - soulCam->Position().x,
				Position().z - soulCam->Position().z);
		}
		else { // target_Default�϶��� �����ο� ī�޶� ȸ�� ����
			soulCam->Rotation().y = atan2f( // ȸ���� Root�� ȸ��, �� ī�޶� yȸ������ �����ص� ���X
				Position().x - soulCam->Position().x,
				Position().z - soulCam->Position().z);
		}

		// x�� ȸ��
		soulCam->Rotation().x += ImGui::GetIO().MouseDelta.y * rotationSpeed * DELTA; // x�� ȸ���� ī�޶�
		soulCam->Rotation().x = Calc::Clamp(soulCam->Rotation().x, -XM_PI / 4.0f, XM_PI / 5.0f);

		camPos->Position().y = 1.2f + 2.0f * soulCam->Rotation().x; // ȸ������ ���� ������ ����
		camPos->Position().y = Calc::Clamp(camPos->Position().y, 0.0f, 20.0f); // ���� �ִ� �ּ� ������

		Root->Rotation().y += ImGui::GetIO().MouseDelta.x * rotationSpeed * DELTA; // �⺻ ���콺 y�� ȸ��
		Root->Rotation().y = Calc::RepeatPI(Root->Rotation().y); // Repeat
	}

	// �̵�
	if (t != nullptr) height = t->getHeight(Position().x, Position().z);
	Position().y = Calc::Lerp(Position().y, height, 0.1f);

	if (model->isStep()) {
		if (move_dir.x == 0 && move_dir.z == 0) move_dir = model->Forward();
		moveSpeed = stepSpeed;
		Position() += move_dir * moveSpeed * DELTA;
		return;
	}

	if (MOUSE_CLICK(0)) { model->animate_Attack(); return; }

	move_dir = Vector3::zero(); // �̵����� �ʱ�ȭ (normalize()�� �ϱ� ������ 0���� �ʱ�ȭ)

	if (KEY_PRESS(ImGuiKey_Space)) moveSpeed = Calc::Lerp(moveSpeed, sprintSpeed, DELTA); // �̼� ����
	else moveSpeed = Calc::Lerp(moveSpeed, walkSpeed, DELTA); // �̼� ȸ��

	if (KEY_PRESS(ImGuiKey_W)) move_dir += soulCam->Forward();
	if (KEY_PRESS(ImGuiKey_S)) move_dir -= soulCam->Forward();
	if (KEY_PRESS(ImGuiKey_A)) move_dir -= soulCam->Right  ();
	if (KEY_PRESS(ImGuiKey_D)) move_dir += soulCam->Right  ();
	move_dir = move_dir.normalize(); // �̵����� �Ϲ�ȭ (�밢�� Ű�Է� ����)

	if (move_dir.x != 0.0f || move_dir.z != 0.0f) { // �����϶� : ���� ȸ���������� �ε巴�� ȸ��
		moveRotation = atan2f(-move_dir.x, -move_dir.z);

		model_rot = (target != nullptr) ? // Ÿ���� ������
			((moveSpeed < stepSpeed) ?
				Root->Rotation().y : // Ÿ�� ��������
				moveRotation) : // �׷��� �̵��ӵ��� ������ �̵���������
			moveRotation; // Ÿ���� ������ �̵� ��������
		model_rot += (model->Rotation().y < 0) ? // ȸ������ �ʰ��� �˻�
			((model_rot - model->Rotation().y > XM_PI) ? -XM_2PI : 0.0f) :
			((model->Rotation().y - model_rot > XM_PI) ? +XM_2PI : 0.0f);
		model->Rotation().y = Calc::Lerp(model->Rotation().y, model_rot, DELTA / 0.3f); // ����
		model->Rotation().y = Calc::Repeat(model->Rotation().y, -XM_PI, XM_PI); // �ʰ��� Repeat
	}
	else moveSpeed = 1.99f;

	if (model->isAttack()) { return; } // �������ϰ�� Ż��

	if (KEY_UP(ImGuiKey_Space)) {
		if (moveSpeed < stepSpeed) { // �̵��ӵ��� ������� ����ȭ ������
			model->animate_Step(moveRotation);
			SP_Bar->SetValue((stat.SP -= 10.0f) / stat.MaxSP);
			return;
		}
	}
	Position() += move_dir * moveSpeed * DELTA; // �̵� ����

	model->animate_Move(moveRotation, moveSpeed); // �̵� ���⿡ ���� �̵� �ִϸ��̼� �۵�
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
