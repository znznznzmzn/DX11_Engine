#pragma once


// Souls�� ���� �� ���ۿ����� ���ǰ��� Model���� ����,
// Model�� ���ǿ� �´� Animation�� ����
class Controller_Souls : public Transform {
private:
	C_PotMan* model;
	Camera* soulCam;

	Transform* Root;
	Transform* camPos;

	Transform* target; // ���� ->>> Ÿ�ٿ��̴�.
	Sphere* Targeter; // target�� ������ ������ ��� �ش� ��ġ�� ��ü ǥ��
	//�ð��� �ȴٸ� 2�� ī�޶�� �����ؼ� 3D�� �������, othograh�� �Ÿ���� ���� ǥ���غ���...

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