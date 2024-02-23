#pragma once

// 애니메이션만 실행
class C_PotMan : public ModelAnimator {
private:
	enum ActionState : UINT {
		IDLE, MOVE_FORWARD, MOVE_BACK, MOVE_LEFT, MOVE_RIGHT, SPRINT, 
		GRAB, STEP_FORWARD, STEP_BACK, STEP_LEFT, STEP_RIGHT, ACTION,
		ATTACK_1, ATTACK_2, ATTACK_3, ATTACK_4, DAMAGE_1, DAMAGE_2, DEAD
	} curAction = ActionState::IDLE;

	Model* pot;
	Matrix pot_mtx;
	Model* knife;
	Matrix knife_mtx;

	void SetAction(ActionState state) { if (curAction != state) PlayClip(curAction = state); }
	void Action(ActionState state, float speed = 1.0f, float takeTime = 0.2f);

	void stepEnd();

	ActionState attackIDX = IDLE;
	void nextAttack();

	bool isHitBoxActive = true;

	vector<Matrix> collisions_Body_mtx;
public:
	vector<Collider*> collisions_Body;
	vector<Collider*> collisions_Weapon;

	C_PotMan();
	~C_PotMan();

	void Update();
	void Render();
	void GUIRender();

	UINT GetStateNumber() { return static_cast<UINT>(curAction); }
	bool isStep  () { return  STEP_FORWARD <= curAction && curAction <= STEP_RIGHT; }
	bool isAttack() { return  ATTACK_1 <= curAction && curAction <= ATTACK_4; }

	void animate_Attack();
	void animate_Step(float const& step_direction);
	void animate_Move(float const& move_rot_y, float const& speed);

	void SetHitBoxActive(bool isActive);
	bool GetHitBoxActive() { return isHitBoxActive; }
};