#pragma once

class FPS_Runner : public C_FirstPerson {
protected:
	float walk_time = 0.0f; // �ȴ� �ð�
	float walk_vibe = 1.6f; // ��鸲 �ӵ�
	float walk_dist = 0.2f; // ��鸲 ����
	Vector3 walk_damp = { 0.0f, 0.0f, 0.0f }; // �̵��� ��鸲

	Status stat; // �⺻ �������ͽ�

	void Move()   override; // ������
	void Rotate() override; // ȸ��   

	GUI_Basic* HP_ico;
	GUI_ProgressBar* HP_Bar;

	SphereCollider* BodyCollider;
	PointLight* Lantern;
public:
	FPS_Runner();
	~FPS_Runner();

	void UpdateWorld() override;

	void Update() override;
	void Render();	// Light
	void PostRender();	// GUI
	void GUIRender() override; // EditorGUI

	SphereCollider*& GetCollision() { return BodyCollider; }
	Camera*& GetCamera() { return FirstCam; }
};