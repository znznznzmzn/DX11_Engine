#pragma once

class FPS_Runner : public C_FirstPerson {
protected:
	float walk_time = 0.0f; // 걷는 시간
	float walk_vibe = 1.6f; // 흔들림 속도
	float walk_dist = 0.2f; // 흔들림 세기
	Vector3 walk_damp = { 0.0f, 0.0f, 0.0f }; // 이동시 흔들림

	Status stat; // 기본 스테이터스

	void Move()   override; // 움직임
	void Rotate() override; // 회전   

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