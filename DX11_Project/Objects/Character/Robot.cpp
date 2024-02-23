#include "Framework.h"

Robot::Robot() {
	#pragma region Body
	{
		float w2 = 0.4f;
		float h2 = 0.5f;
		float d2 = 0.2f;
		// front
		vertices.emplace_back(VertexType(-w2, -h2, -d2, 0, 1));
		vertices.emplace_back(VertexType(-w2, +h2, -d2, 0, 0));
		vertices.emplace_back(VertexType(+w2, +h2, -d2, 1, 0));
		vertices.emplace_back(VertexType(+w2, -h2, -d2, 1, 1));
		// back				  
		vertices.emplace_back(VertexType(-w2, -h2, +d2, 1, 1));
		vertices.emplace_back(VertexType(+w2, -h2, +d2, 0, 1));
		vertices.emplace_back(VertexType(+w2, +h2, +d2, 0, 0));
		vertices.emplace_back(VertexType(-w2, +h2, +d2, 1, 0));
		// top				  
		vertices.emplace_back(VertexType(-w2, +h2, -d2, 0, 1));
		vertices.emplace_back(VertexType(-w2, +h2, +d2, 0, 0));
		vertices.emplace_back(VertexType(+w2, +h2, +d2, 1, 0));
		vertices.emplace_back(VertexType(+w2, +h2, -d2, 1, 1));
		// bottom			  
		vertices.emplace_back(VertexType(-w2, -h2, -d2, 1, 1));
		vertices.emplace_back(VertexType(+w2, -h2, -d2, 0, 1));
		vertices.emplace_back(VertexType(+w2, -h2, +d2, 0, 0));
		vertices.emplace_back(VertexType(-w2, -h2, +d2, 1, 0));
		// left				  
		vertices.emplace_back(VertexType(-w2, -h2, +d2, 0, 1));
		vertices.emplace_back(VertexType(-w2, +h2, +d2, 0, 0));
		vertices.emplace_back(VertexType(-w2, +h2, -d2, 1, 0));
		vertices.emplace_back(VertexType(-w2, -h2, -d2, 1, 1));
		// right			  
		vertices.emplace_back(VertexType(+w2, -h2, -d2, 0, 1));
		vertices.emplace_back(VertexType(+w2, +h2, -d2, 0, 0));
		vertices.emplace_back(VertexType(+w2, +h2, +d2, 1, 0));
		vertices.emplace_back(VertexType(+w2, -h2, +d2, 1, 1));
	}
	indices = {
		0 , 1 , 2 , 0 , 2 , 3 ,
		4 , 5 , 6 , 4 , 6 , 7 ,
		8 , 9 , 10, 8 , 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};
	Calc::Vertex::Normal(vertices, indices);
	Calc::Vertex::Tangent(vertices, indices);
	tag = "Robot";
	#pragma endregion

	#pragma region Head
	Joint_Neck = new Sphere(Joint_Slice);
	Joint_Neck->SetParent(this);
	Joint_Neck->Position().y += 0.55f;
	Joint_Neck->Scale() = Vector3(0.2f, 0.2f, 0.2f);
	Head = new Sphere(20);
	Head->GetMaterial()->SetDiffuseMap (L"Textures/sad_cat.png");
	Head->GetMaterial()->SetSpecularMap(L"Textures/sad_cat_specular.png");
	Head->GetMaterial()->SetNormalMap  (L"Textures/sad_cat_normal.png");
	Head->SetParent(Joint_Neck);
	Head->Position().y += 1.0f;
	Head->Rotation().y = 1.5f;
	Head->Scale() = Vector3(2.7f, 2.7f, 2.7f);
	#pragma endregion

	#pragma region Arm
	// Left
	Joint_Arm_Left   = new Sphere(Joint_Slice);
	Joint_Arm_Left->SetParent(this);
	Joint_Arm_Left->Scale() = Vector3(0.3f, 0.3f, 0.3f);
	Joint_Arm_Left->Position() = Vector3(-0.5f, 0.4f, 0.0f);
	Arm_Left = new Cylinder();
	Arm_Left->SetParent(Joint_Arm_Left);
	Arm_Left->Position().y = -1.0f;
	Joint_Elbow_Left = new Sphere(Joint_Slice);
	Joint_Elbow_Left->SetParent(Arm_Left);
	Joint_Elbow_Left->Position().y = -1.0f;
	Forearm_Left = new Cylinder();
	Forearm_Left->SetParent(Joint_Elbow_Left);
	Forearm_Left->Position().y = -1.0f;
	Hand_Left = new Sphere();
	Hand_Left->SetParent(Forearm_Left);
	Hand_Left->Position().y = -1.0f;
	// Right
	Joint_Arm_Right   = new Sphere(Joint_Slice);
	Joint_Arm_Right->SetParent(this);
	Joint_Arm_Right->Scale() = Vector3(0.3f, 0.3f, 0.3f);
	Joint_Arm_Right->Position() = Vector3(0.5f, 0.4f, 0.0f);
	Arm_Right = new Cylinder();
	Arm_Right->SetParent(Joint_Arm_Right);
	Arm_Right->Position().y = -1.0f;
	Joint_Elbow_Right = new Sphere(Joint_Slice);
	Joint_Elbow_Right->SetParent(Arm_Right);
	Joint_Elbow_Right->Position().y = -1.0f;
	Forearm_Right = new Cylinder();
	Forearm_Right->SetParent(Joint_Elbow_Right);
	Forearm_Right->Position().y = -1.0f;
	Hand_Right = new Sphere();
	Hand_Right->SetParent(Forearm_Right);
	Hand_Right->Position().y = -1.0f;
	#pragma endregion
	
	#pragma region Leg
	// Left
	Joint_Pelvis_Left = new Sphere(Joint_Slice);	// 골반 
	Joint_Pelvis_Left->SetParent(this);
	Joint_Pelvis_Left->Scale() = Vector3(0.3f, 0.3f, 0.3f);
	Joint_Pelvis_Left->Position() = Vector3(-0.2f, -0.55f, 0.0f);
	Thigh_Left = new Cylinder();
	Thigh_Left->SetParent(Joint_Pelvis_Left);
	Thigh_Left->Position().y = -1.0f;
	Joint_Leg_Left = new Sphere(Joint_Slice);
	Joint_Leg_Left->SetParent(Thigh_Left);
	Joint_Leg_Left->Position().y = -1.0f;
	Shin_Left = new Cylinder(); // 정강이
	Shin_Left->SetParent(Joint_Leg_Left);
	Shin_Left->Position().y = -1.0f; 
	Feet_Left = new Cube();
	Feet_Left->SetParent(Shin_Left);
	Feet_Left->Scale() = Vector3(1.0f, 0.7f, 1.5f);
	Feet_Left->Position() = Vector3(0.0f, -0.8f, 0.3f);
	// Right
	Joint_Pelvis_Right = new Sphere(Joint_Slice);	// 골반 
	Joint_Pelvis_Right->SetParent(this);
	Joint_Pelvis_Right->Scale() = Vector3(0.3f, 0.3f, 0.3f);
	Joint_Pelvis_Right->Position() = Vector3(0.2f, -0.55f, 0.0f);
	Thigh_Right = new Cylinder();
	Thigh_Right->SetParent(Joint_Pelvis_Right);
	Thigh_Right->Position().y = -1.0f;
	Joint_Leg_Right = new Sphere(Joint_Slice);
	Joint_Leg_Right->SetParent(Thigh_Right);
	Joint_Leg_Right->Position().y = -1.0f;
	Shin_Right = new Cylinder(); // 정강이
	Shin_Right->SetParent(Joint_Leg_Right);
	Shin_Right->Position().y = -1.0f;
	Feet_Right = new Cube();
	Feet_Right->SetParent(Shin_Right);
	Feet_Right->Scale() = Vector3(1.0f, 0.7f, 1.5f);
	Feet_Right->Position() = Vector3(0.0f, -0.8f, 0.3f);
	#pragma endregion

	init();
}

Robot::~Robot() {// transform == 부모노드가 삭제되면 자동으로 삭제됨
}

#pragma region Anim
void Robot::Animate() {
	switch (animState){
	case 0: Anim_Idle(); break;
	case 1: Anim_Walk(); break;
	default: Anim_Idle(); break;
	}
}

void Robot::Anim_Idle() {
	//la = ll = 2.0f;
	//ra = rl = 0.0f;
	Joint_Arm_Left->Rotation().x     = Calc::Lerp(Joint_Arm_Left->Rotation().x, 0,     DELTA / 0.1f);
	Joint_Elbow_Left->Rotation().x   = Calc::Lerp(Joint_Elbow_Left->Rotation().x, 0,   DELTA / 0.1f);
	Joint_Arm_Right->Rotation().x    = Calc::Lerp(Joint_Arm_Right->Rotation().x, 0,    DELTA / 0.1f);
	Joint_Elbow_Right->Rotation().x  = Calc::Lerp(Joint_Elbow_Right->Rotation().x, 0,  DELTA / 0.1f);
	Joint_Pelvis_Left->Rotation().x  = Calc::Lerp(Joint_Pelvis_Left->Rotation().x, 0,  DELTA / 0.1f);
	Joint_Leg_Left->Rotation().x     = Calc::Lerp(Joint_Leg_Left->Rotation().x, 0,     DELTA / 0.1f);
	Joint_Pelvis_Right->Rotation().x = Calc::Lerp(Joint_Pelvis_Right->Rotation().x, 0, DELTA / 0.1f);
	Joint_Leg_Right->Rotation().x    = Calc::Lerp(Joint_Leg_Right->Rotation().x, 0,    DELTA / 0.1f);
}

void Robot::Anim_Walk() {
	la += DELTA * 5.0f;
	ra += DELTA * 5.0f;
	ll += DELTA * 5.0f;
	rl += DELTA * 5.0f;
	Joint_Arm_Left->Rotation().x     = Calc::Lerp(Joint_Arm_Left->Rotation().x, cos(la),     DELTA / 0.1f); 
	Joint_Elbow_Left->Rotation().x   = Calc::Lerp(Joint_Elbow_Left->Rotation().x, sin(la),   DELTA / 0.1f);
	Joint_Arm_Right->Rotation().x    = Calc::Lerp(Joint_Arm_Right->Rotation().x, cos(ra),    DELTA / 0.1f);
	Joint_Elbow_Right->Rotation().x  = Calc::Lerp(Joint_Elbow_Right->Rotation().x, sin(ra),  DELTA / 0.1f);
	Joint_Pelvis_Left->Rotation().x  = Calc::Lerp(Joint_Pelvis_Left->Rotation().x, cos(ll),  DELTA / 0.1f);
	Joint_Leg_Left->Rotation().x     = Calc::Lerp(Joint_Leg_Left->Rotation().x, sin(ll),     DELTA / 0.1f);
	Joint_Pelvis_Right->Rotation().x = Calc::Lerp(Joint_Pelvis_Right->Rotation().x, cos(rl), DELTA / 0.1f);
	Joint_Leg_Right->Rotation().x    = Calc::Lerp(Joint_Leg_Right->Rotation().x, sin(rl),    DELTA / 0.1f);
}
#pragma endregion

#pragma region Control
/*
void Robot::Control() {
	move_direction = Vector3::zero();
	if (KEY_PRESS(ImGuiKey_UpArrow))  move_direction += Forward();
	if (KEY_PRESS(ImGuiKey_DownArrow)) move_direction -= Forward();
	if (KEY_PRESS(ImGuiKey_RightArrow)) Rotation().y += rotation_speed * DELTA;
	if (KEY_PRESS(ImGuiKey_LeftArrow))  Rotation().y -= rotation_speed * DELTA;
	move_direction += move_direction * move_speed * DELTA;
	Anim();
	animState = 0;
}*/

void Robot::Move() {
	if (path.empty()) {
		animState = 0;
		return;
	}
	else animState = 1;

	Vector3 dest = path.back();
	if (Vector3::distance(Position(), dest) < distance) path.pop_back();
	

	Position() += Forward() * move_speed * DELTA; // 이동 
	if (t != nullptr) Position().y = t->getHeight(Position()) + 2.0f; // y값 받아오기
	move_rotation = atan2f(dest.x - Position().x, dest.z - Position().z); // 회전의 최종 방향
	move_rotation += (Rotation().y < 0) ? // 회전방향 초과분 검사
		((move_rotation - Rotation().y > XM_PI) ? -XM_2PI : 0.0f) :
		((Rotation().y - move_rotation > XM_PI) ? +XM_2PI : 0.0f);
	Rotation().y = Calc::Lerp(Rotation().y, move_rotation, rotation_speed * DELTA); // 해당 방향으로 부드럽게 회전
	Rotation().y = Calc::Repeat(Rotation().y, -XM_PI, XM_PI); // 초과분 Repeat
}
#pragma endregion

void Robot::Render() {
	GameObject::Render();
	Joint_Neck->Render();
	Head->Render();

	Joint_Arm_Left->Render();
	Arm_Left->Render();
	Joint_Elbow_Left->Render();
	Forearm_Left->Render();
	Hand_Left->Render();

	Joint_Arm_Right->Render();
	Arm_Right->Render();
	Joint_Elbow_Right->Render();
	Forearm_Right->Render();
	Hand_Right->Render();

	Joint_Pelvis_Left->Render();
	Thigh_Left->Render();
	Joint_Leg_Left->Render();
	Shin_Left->Render();
	Feet_Left->Render();

	Joint_Pelvis_Right->Render();
	Thigh_Right->Render();
	Joint_Leg_Right->Render();
	Shin_Right->Render();
	Feet_Right->Render();
}

void Robot::UpdateWorld() {
	Transform::UpdateWorld();
	Joint_Neck->UpdateWorld();
	Head->UpdateWorld();

	Joint_Arm_Left->UpdateWorld();
	Arm_Left->UpdateWorld();
	Joint_Elbow_Left->UpdateWorld();
	Forearm_Left->UpdateWorld();
	Hand_Left->UpdateWorld();

	Joint_Arm_Right->UpdateWorld();
	Arm_Right->UpdateWorld();
	Joint_Elbow_Right->UpdateWorld();
	Forearm_Right->UpdateWorld();
	Hand_Right->UpdateWorld();

	Joint_Pelvis_Left->UpdateWorld();
	Thigh_Left->UpdateWorld();
	Joint_Leg_Left->UpdateWorld();
	Shin_Left->UpdateWorld();
	Feet_Left->UpdateWorld();

	Joint_Pelvis_Right->UpdateWorld();
	Thigh_Right->UpdateWorld();
	Joint_Leg_Right->UpdateWorld();
	Shin_Right->UpdateWorld();
	Feet_Right->UpdateWorld();
}
