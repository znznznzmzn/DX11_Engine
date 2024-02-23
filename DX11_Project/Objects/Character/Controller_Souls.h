#pragma once


// Souls는 조작 및 조작에따른 조건값을 Model에개 전달,
// Model은 조건에 맞는 Animation을 수행
class Controller_Souls : public Transform {
private:
	C_PotMan* model;
	Camera* soulCam;

	Transform* Root;
	Transform* camPos;

	Transform* target; // 주의 ->>> 타겟용이다.
	Sphere* Targeter; // target이 적군을 집었을 경우 해당 위치에 구체 표시
	//시간이 된다면 2개 카메라로 구성해서 3D에 상관없이, othograh로 거리상관 없이 표현해보자...

	float height = 0.0f;
	Terrain* t = nullptr;

	Vector3 move_dir = { 0.0f, 0.0f, 0.0f };
	float moveRotation = 0.0f;
	float moveSpeed = 0.0f;
	const float walkSpeed = 2.0f;
	const float sprintSpeed = 4.5f;
	const float stepSpeed = 3.5f;
	const float jumpAdderSpeed = 1.0f;
	float rotationSpeed = Calc::PI;

	float model_rot = 0.0f;

	GUI_ProgressBar* HP_Bar;
	GUI_ProgressBar* SP_Bar;
public:
	Controller_Souls();
	~Controller_Souls();

	void Control();
	void PreRender();
	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void SetTarget(Transform* target);
	void SetGround(Terrain*& target) { t = target; }
	vector<Collider*> GetCollision_Body   () { return model->collisions_Body  ; }
	vector<Collider*> GetCollision_Weapons() { return model->collisions_Weapon; }

	Status stat;
};