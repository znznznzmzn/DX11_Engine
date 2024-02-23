#pragma once

class BoxCollider : public Collider {
private:
	struct ObbDesc {
		Vector3 position;
		Vector3 axis[3];
		Vector3 halfSize;
	} thisBox, targetBox;
	Vector3 size;

	bool IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc box1, ObbDesc box2);
public:
	BoxCollider(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));

	// Collider을(를) 통해 상속됨
	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;

	void GetObb(OUT ObbDesc& obbDesc);
};

