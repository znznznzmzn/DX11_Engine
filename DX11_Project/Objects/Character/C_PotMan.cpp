#include "Framework.h"

C_PotMan::C_PotMan() : ModelAnimator("PotMan") {
	Scale() = { 0.01f, 0.01f, 0.01f }; // 기본 사이즈
	// 장착물
	pot = new Model("Pot");
	pot->SetParent(&pot_mtx);
	pot->Position() = { 0.0f , 44.7f, 2.05f };
	pot->Rotation() = { XM_PI,  0.0f,  0.0f };
	pot->Scale()	= { 1.5f , 1.45f,  1.5f };

	knife = new Model("Knife");
	knife->SetParent(&knife_mtx);
	knife->Position() = { 7.4f, 5.8f, -2.0f };
	knife->Rotation() = { 0.0f, 1.6f,  1.7f };
	knife->Scale()	  = { 2.5f, 2.5f,  2.5f };

	// Collision
	{ // 무기
		BoxCollider* collision_box = new BoxCollider();
		collision_box->Tag() = "weapon_knife";
		collision_box->Position().z = -10.8f;
		collision_box->Scale()    = { 2.3f, 1.0f, 15.0f };
		collision_box->SetParent(knife);
		collision_box->SetActive(false);
		collisions_Weapon.emplace_back(collision_box);
	}
	{ // 몸체
		BoxCollider* collision_box;
		SphereCollider* collision_sph;

		// head
		collision_sph = new SphereCollider();
		collision_sph->Tag() = "head";
		collision_sph->Position() = {  0.0f,  9.9f, -1.5f };
		collision_sph->Scale   () = { 14.0f, 14.0f, 14.0f };
		collisions_Body.emplace_back(collision_sph);

		// body
		collision_box = new BoxCollider();
		collision_box->Tag() = "body";
		collision_box->Position().y = 2.7f;
		collision_box->Scale() = { 35.0f, 85.0f, 20.0f };
		collisions_Body.emplace_back(collision_box);

		// arm_L
		collision_box = new BoxCollider();
		collision_box->Tag() = "arm_L";
		collision_box->Position().y = 6.0f;
		collision_box->Scale() = { 10.0f, 40.0f, 10.0f };
		collisions_Body.emplace_back(collision_box);
		// elbow_L
		collision_box = new BoxCollider();
		collision_box->Tag() = "elbow_L";
		collision_box->Position().y = 11.0f;
		collision_box->Scale() = { 8.0f, 50.0f, 8.0f };
		collisions_Body.emplace_back(collision_box);

		// arm_R
		collision_box = new BoxCollider();
		collision_box->Tag() = "arm_R";
		collision_box->Position().y = 6.0f;
		collision_box->Scale() = { 10.0f, 40.0f, 10.0f };
		collisions_Body.emplace_back(collision_box);
		// elbow_R
		collision_box = new BoxCollider();
		collision_box->Tag() = "elbow_R";
		collision_box->Position().y = 11.0f;
		collision_box->Scale() = { 8.0f, 50.0f, 8.0f };
		collisions_Body.emplace_back(collision_box);

		// pelvis_L
		collision_box = new BoxCollider();
		collision_box->Tag() = "pelvis_L";
		collision_box->Position().y = 9.0f;
		collision_box->Scale() = { 15.0f, 55.0f, 15.0f };
		collisions_Body.emplace_back(collision_box);
		// knee_L
		collision_box = new BoxCollider();
		collision_box->Tag() = "knee_L";
		collision_box->Position().y = 18.0f;
		collision_box->Scale() = { 10.0f, 65.0f, 10.0f };
		collisions_Body.emplace_back(collision_box);
		// feet_L
		collision_box = new BoxCollider();
		collision_box->Position() = { 0.0f, -4.5f, -2.5f };
		collision_box->Scale()    = { 15.0f, 39.0f, 8.0f };
		collision_box->Tag() = "feet_L";
		collisions_Body.emplace_back(collision_box);

		// pelvis_R
		collision_box = new BoxCollider();
		collision_box->Tag() = "pelvis_R";
		collision_box->Position().y = 9.0f;
		collision_box->Scale() = { 15.0f, 55.0f, 15.0f };
		collisions_Body.emplace_back(collision_box);
		// knee_R
		collision_box = new BoxCollider();
		collision_box->Tag() = "knee_R";
		collision_box->Position().y = 18.0f;
		collision_box->Scale() = { 10.0f, 65.0f, 10.0f };
		collisions_Body.emplace_back(collision_box);
		// feet_R
		collision_box = new BoxCollider();
		collision_box->Tag() = "feet_R";
		collision_box->Position() = { 0.0f, -4.5f, -2.5f };
		collision_box->Scale()    = { 15.0f, 39.0f, 8.0f };
		collisions_Body.emplace_back(collision_box);

		collisions_Body_mtx.resize(collisions_Body.size());
		for (UINT i = 0; i < collisions_Body.size(); i++) // 매트릭스별로 부모 적용
			collisions_Body[i]->SetParent(&collisions_Body_mtx[i]);
	}

	// 애니메이션
	// Base Move
	ReadClip("PotMan_Idle"); // 애니메이션 받아오기
	ReadClip("PotMan_Walk_Back");
	ReadClip("PotMan_Walk_Forward");
	ReadClip("PotMan_Walk_Left");
	ReadClip("PotMan_Walk_Right");
	ReadClip("PotMan_Sprint");

	ReadClip("PotMan_GrabItem");
	ReadClip("PotMan_Step_Forward");
	ReadClip("PotMan_Step_Back");
	ReadClip("PotMan_Step_Left");
	ReadClip("PotMan_Step_Right");
	ReadClip("PotMan_Action");

	ReadClip("PotMan_Attack_1");
	ReadClip("PotMan_Attack_2");
	ReadClip("PotMan_Attack_3");
	ReadClip("PotMan_Attack_4");
	ReadClip("PotMan_Damage_1");
	ReadClip("PotMan_Damage_2");
	ReadClip("PotMan_Dead");

	// 애니메이션 이벤트 설정
	clips[STEP_FORWARD]->SetEvents(bind(&C_PotMan::stepEnd, this), 0.2f); // 구르기 무적 종료
	clips[STEP_BACK   ]->SetEvents(bind(&C_PotMan::stepEnd, this), 0.2f);
	clips[STEP_LEFT   ]->SetEvents(bind(&C_PotMan::stepEnd, this), 0.2f);
	clips[STEP_RIGHT  ]->SetEvents(bind(&C_PotMan::stepEnd, this), 0.2f);
	
	clips[ATTACK_1]->SetEvents(bind(&C_PotMan::nextAttack, this), clips[ATTACK_1]->GetEndTime() - 0.7f); // 다음 공격 세팅
	clips[ATTACK_2]->SetEvents(bind(&C_PotMan::nextAttack, this), clips[ATTACK_2]->GetEndTime() - 0.7f);
	clips[ATTACK_3]->SetEvents(bind(&C_PotMan::nextAttack, this), clips[ATTACK_3]->GetEndTime() - 0.7f);
	clips[ATTACK_4]->SetEvents(bind(&C_PotMan::nextAttack, this), clips[ATTACK_4]->GetEndTime() - 0.7f);

	PlayClip(IDLE);
}
C_PotMan::~C_PotMan() {
	for (Collider*& elem : collisions_Body  ) { SAFE_DELETE(elem); }
	for (Collider*& elem : collisions_Weapon) { SAFE_DELETE(elem); }
	SAFE_DELETE(pot);
	SAFE_DELETE(knife);
}

void C_PotMan::Update() {
	// Anim
	UpdateFrame();

	// Models
	pot_mtx = GetTransformByNode(7) * world;
	pot->UpdateWorld();

	knife_mtx = GetTransformByNode(12) * world;
	knife->UpdateWorld();

	// Collisions
	if (isHitBoxActive) { // 히트박스가 켜져있을때, 각 충돌 매트릭스 잡아오기
		collisions_Body_mtx[ 0] = GetTransformByNode( 7) * world; // head
		collisions_Body_mtx[ 1] = GetTransformByNode( 4) * world; // body
		collisions_Body_mtx[ 2] = GetTransformByNode(10) * world; // arm_L
		collisions_Body_mtx[ 3] = GetTransformByNode(11) * world; // elbow_L
		collisions_Body_mtx[ 4] = GetTransformByNode(34) * world; // arm_R
		collisions_Body_mtx[ 5] = GetTransformByNode(35) * world; // elbow_R
		collisions_Body_mtx[ 6] = GetTransformByNode(57) * world; // pelvis_L
		collisions_Body_mtx[ 7] = GetTransformByNode(58) * world; // knee_L
		collisions_Body_mtx[ 8] = GetTransformByNode(60) * world; // feet_L
		collisions_Body_mtx[ 9] = GetTransformByNode(62) * world; // pelvis_R
		collisions_Body_mtx[10] = GetTransformByNode(63) * world; // knee_R
		collisions_Body_mtx[11] = GetTransformByNode(65) * world; // feet_R
		for (Collider*& elem : collisions_Body) elem->UpdateWorld(); // 적용한대로 각각 UpdateWorld
	}
	for (Collider*& elem : collisions_Weapon) elem->UpdateWorld();
}
// TransformIDX
// body - 4
// neck - 7
// hair_top - 8
// arm_L - 10
// elbow_L - 11
// wrist_L - 12
// 
// arm_R - 34
// elbow_R - 35
// wrist_R - 36
// 
// 골반_L - 57
// 무릎_L - 58
// 발목_L - 59
// 
// 골반_R - 62
// 무릎_R - 63
// 발목_R - 64
//

void C_PotMan::Render() {
	pot->Render();
	knife->Render();
	ModelAnimator::Render();

	// 우선 에디팅 수준에서 맞춰놓기 위해 이렇게만 세팅
	for (Collider*& elem : collisions_Body  ) elem->Render();
	for (Collider*& elem : collisions_Weapon) elem->Render();
}

void C_PotMan::GUIRender() {
	Transform::GUIRender();
}

void C_PotMan::Action(ActionState state, float speed, float takeTime)
{ if (curAction != state) PlayClip(curAction = state, speed, takeTime); }

void C_PotMan::stepEnd() {
	SetHitBoxActive(true);
	Position().y = 0;
	SetAction(IDLE);
}

void C_PotMan::nextAttack() {
	collisions_Weapon[0]->SetActive(false); // 충돌 제거
	if (attackIDX == curAction || attackIDX > ATTACK_4) { // 다음 공격 인덱스가 없거나, 최대 공격 인덱스에 달했다면
		SetAction(attackIDX = IDLE); // 일반상태로 복귀 (공격인덱스 초기화)
	}
	else if (attackIDX > curAction) { // 다음 공격이 활성화 상태라면
		Action(attackIDX, 1.5f, 0.2f); // 다음 공격 진행
		collisions_Weapon[0]->SetActive(true); // 충돌 복귀
	}
}

void C_PotMan::animate_Attack() {
	if (isAttack()) attackIDX = (ActionState)(curAction + 1); // 공격중일 경우, 공격 인덱스 + 1
	else { // 초기 공격
		collisions_Weapon[0]->SetActive(true);
		Action(ATTACK_1, 1.0f, 0.2f);
	}
}
void C_PotMan::animate_Step(float const& step_direction) {
	SetHitBoxActive(false);

	float dir = Calc::RepeatPI(step_direction - Rotation().y);
	if (dir > +XM_PI * 3 / 4 || dir < -XM_PI * 3 / 4) Action(STEP_BACK, 1.2f, 0.3f);
	else if (dir < -XM_PI / 4) Action(STEP_LEFT , 1.2f, 0.3f);
	else if (dir > +XM_PI / 4) Action(STEP_RIGHT, 1.2f, 0.3f);
	else Action(STEP_FORWARD, 1.2f, 0.3f);
}
void C_PotMan::animate_Move(float const& move_rot_y, float const& speed) {
	if (speed <= 1.99f) { SetAction(IDLE); return; }

	float dir = Calc::RepeatPI(move_rot_y - Rotation().y);
	if (speed > 3.6f) Action(SPRINT, 1.0f, 0.3f);
	else if (dir > +XM_PI * 3 / 4 || dir < -XM_PI * 3 / 4) Action(MOVE_BACK, 1.5f, 0.3f);
	else if (dir < -XM_PI / 4) Action(MOVE_LEFT, 1.5f, 0.3f);
	else if (dir > +XM_PI / 4) Action(MOVE_RIGHT, 1.5f, 0.3f);
	else Action(MOVE_FORWARD, 1.5f, 0.3f);
}

void C_PotMan::SetHitBoxActive(bool isActive) {
	isHitBoxActive = isActive;
	for (Collider*& elem : collisions_Body) elem->SetActive(isHitBoxActive);
}
