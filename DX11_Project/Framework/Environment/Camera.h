#pragma once

class Camera : public Transform {
private:
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix invView;

	Vector3 direction = { 0.0f, 0.0f, 0.0f };
	float move_speed = 15.0f;
	float move_speed_normal = 10.0f;
	float move_speed_dash = 30.0f;
	float rotation_speed = 5.0f;

	float destRot = 0.0f;

	void FreeMode();
public:
	Camera();
	~Camera();
	void UpdateWorld() override;
	void Update();
	void SetViewBuffer();
	void PreRender() { SetViewBuffer(); }
	void GUIRender();
	Ray ScreenPointToRay(Vector3 screenPos);
	Vector3 WorldToScreenPoint(Vector3 worldPos);
	void LookAt(Vector3 target, Vector3 upVector = Vector3::up());

	bool is_freemode = false;
};