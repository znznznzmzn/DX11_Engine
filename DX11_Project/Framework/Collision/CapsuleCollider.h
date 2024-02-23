#pragma once
class CapsuleCollider : public Collider {
private: 
	float height = 2.0f;
public:
	CapsuleCollider(float height = 2.0f);
	~CapsuleCollider();
	// Collider을(를) 통해 상속됨
	virtual bool RayCollision(IN Ray ray, OUT Contact* contact) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return max(GlobalScale().x, max(GlobalScale().y, GlobalScale().z)); };
	float Height() { return height * GlobalScale().y; }
};

