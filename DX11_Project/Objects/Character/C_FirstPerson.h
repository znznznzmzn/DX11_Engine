#pragma once

class C_FirstPerson : public Transform {
private:
protected:
	const Vector3 Camera_Offset = { 0.0f, 1.25f, 0.0f };

	Camera* FirstCam;

	virtual void Move();   // 기본 움직임
	virtual void Rotate(); // 기본 회전  

	Vector3 move_direction = { 0.0f, 0.0f, 0.0f };
	float move_speed = 0.0f;
	float walk_speed = 7.0f;
	float dash_speed = 14.0f;
	float rotation_speed = 5.0f;
	Terrain* t = nullptr;
public:
	C_FirstPerson();
	~C_FirstPerson();
	virtual void Update();
	virtual void PreRender();
	virtual void GUIRender();
	virtual void UpdateWorld() override;

	void setTerrain(Terrain* t) { this->t = t; }
};