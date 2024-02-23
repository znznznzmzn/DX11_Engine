#pragma once

struct Ray {
	Vector3 position;
	Vector3 direction;
};

struct Contact {
	Vector3 hitPoint;
	float distance;
};

class BoxCollider;
class SphereCollider;
class CapsuleCollider;

class Collider : public Transform {
private:
	Material* material = nullptr;
	MatrixBuffer* wBuffer = nullptr;
protected:
	Mesh* mesh = nullptr;
	vector<Vertex> vertices;
	vector<UINT> indices;
	void CreateMesh();
public:
	Collider();
	~Collider();

	void Render();

	bool Collision(Collider* collider);

	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) = 0;
	virtual bool BoxCollision(BoxCollider* collider) = 0;
	virtual bool SphereCollision(SphereCollider* collider) = 0;
	virtual bool CapsuleCollision(CapsuleCollider* collider) = 0;

	void SetColor(float4 color) { material->GetData().diffuse = color; }
	void SetColor(float r, float g, float b) { SetColor({ r, g, b, 1.0f }); }

	enum class TYPE : UINT { NONE, POINT, SPHERE, CAPSULE, BOX, MESH };
protected:
	TYPE type = TYPE::NONE;
};