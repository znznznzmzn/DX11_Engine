#pragma once
class Robot : public GameObject {
private:
#pragma region Boies
	Sphere * Head;
	Sphere * Joint_Neck;

	Sphere * Joint_Arm_Left;
	Sphere * Joint_Elbow_Left;
	Cylinder * Arm_Left;
	Cylinder * Forearm_Left;
	Sphere * Hand_Left;

	Sphere * Joint_Arm_Right;
	Sphere * Joint_Elbow_Right;
	Cylinder * Arm_Right;
	Cylinder * Forearm_Right;
	Sphere * Hand_Right;

	Sphere * Joint_Pelvis_Left; // °ñ¹Ý 
	Sphere * Joint_Leg_Left;	// ¹«¸­ 
	Cylinder * Thigh_Left;		// Çã¹÷Áö
	Cylinder * Shin_Left;		// Á¤°­ÀÌ
	Cube * Feet_Left;			// ¹ß

	Sphere * Joint_Pelvis_Right;
	Sphere * Joint_Leg_Right;
	Cylinder * Thigh_Right;
	Cylinder * Shin_Right;
	Cube * Feet_Right;
#pragma endregion

	int Joint_Slice = 6;
	float la = 0.0f;
	float ra = 2.0f;
	float ll = 2.0f;
	float rl = 0.0f;

	const float move_speed = 7.0f;
	const float rotation_speed = 10.0f;

	Vector3 move_direction = Vector3::zero();
	float move_rotation = 0.0f;
	float distance = 3.0f;

	Terrain* t = nullptr;
public: 
	Robot();
	~Robot();

	int animState = 0;
	void Animate();
	void Anim_Idle();
	void Anim_Walk();
	
	void Move();
	void setTerrain(Terrain*& t) { this->t = t; }

	void Render() override;
	void UpdateWorld() override;

	vector<Vector3> path;
};