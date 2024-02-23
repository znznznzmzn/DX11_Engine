#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size) : size(size) {
	Tag() = "BoxCollider";
	type = TYPE::BOX;
	Vector3 halfSize = size * 0.5f;
	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);

	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);

	indices = {
		0, 1, 1, 3, 3, 2, 2, 0,
		4, 5, 5, 7, 7, 6, 6, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	CreateMesh();
}

void BoxCollider::GetObb(OUT ObbDesc& obbDesc) {
	obbDesc.position = GlobalPosition();
	obbDesc.axis[0] = Right();
	obbDesc.axis[1] = Up();
	obbDesc.axis[2] = Forward();
	obbDesc.halfSize = size * 0.5f * GlobalScale();
}
bool BoxCollider::IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc thisBox, ObbDesc targetBox) {
	float a = 0.0f;
	float b = 0.0f;
	for (UINT i = 0; i < 3; i++) {
		a += abs(Vector3::dot(thisBox.axis[i] * thisBox.halfSize[i], axis));
		b += abs(Vector3::dot(targetBox.axis[i] * targetBox.halfSize[i], axis));
	}
	return abs(Vector3::dot(D, axis)) > a + b;
}

bool BoxCollider::RayCollision(IN Ray ray, OUT Contact* contact) {
	UpdateWorld();
	GetObb(thisBox);
	float tMin = 0.0f;
	float tMax = FLT_MAX;
	Vector3 min = thisBox.halfSize * -1.0f;
	Vector3 max = thisBox.halfSize;
	Vector3 delta = thisBox.position - ray.position;
	Vector3 D = ray.direction.normalize();
	for (UINT i = 0; i < 3; i++) {
		Vector3 axis = thisBox.axis[i];
		float e = Vector3::dot(axis, delta);
		float f = Vector3::dot(D, axis);
		if (abs(f) < FLT_EPSILON) {
			if (min[i] > e || max[i] < e) return false;
		} else {
			float t1 = (e + min[i]) / f;
			float t2 = (e + max[i]) / f;
			if (t1 > t2) swap(t1, t2);
			if (t1 > tMin) tMin = t1;
			if (t2 < tMax) tMax = t2;
			if (tMin > tMax) return false;
		}
	}
	if (contact != nullptr) {
		contact->distance = tMin;
		contact->hitPoint = ray.position + ray.direction * tMin;
	}
	return true;
}
bool BoxCollider::BoxCollision(BoxCollider* collider) {
	GetObb(thisBox);
	collider->GetObb(targetBox);
	Vector3 D = targetBox.position - thisBox.position;
	for (UINT i = 0; i < 3; i++) {
		if (IsSeperateAxis(D, thisBox.axis[i]  , thisBox, targetBox)||
			IsSeperateAxis(D, targetBox.axis[i], thisBox, targetBox)) return false;
		for (UINT j = 0; i < 3; i++) {
			if (thisBox.axis[i] == targetBox.axis[j]) return true;
			if (IsSeperateAxis(D,
				Vector3::cross(thisBox.axis[i], targetBox.axis[j]),
				thisBox, targetBox))
				return false;
		}
	}	
	return true;
}
bool BoxCollider::SphereCollision(SphereCollider* collider) {
	GetObb(thisBox);

	Vector3 pos = thisBox.position;

	for (UINT i = 0; i < 3; i++) {
		float length = Vector3::dot(thisBox.axis[i], collider->GlobalPosition() - thisBox.position);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), thisBox.halfSize[i]);
		pos += thisBox.axis[i] * length * mult;
	}
	return Vector3::distance(collider->GlobalPosition(), pos) <= collider->Radius();
}
bool BoxCollider::CapsuleCollision(CapsuleCollider* collider) {
	GetObb(thisBox);
	Vector3 tmpBoxPos = thisBox.position;
	Vector3 CapsuleUpVector = collider->Up();
	Vector3 SphereA = collider->GlobalPosition() - CapsuleUpVector * collider->Height() / 2.0f;
	Vector3 SphereB = collider->GlobalPosition() + CapsuleUpVector * collider->Height() / 2.0f;
	Vector3 pointOnLine = Calc::ClosestPointOnLine(SphereA, SphereB, tmpBoxPos);
	for (UINT i = 0; i < 3; i++) {
		float length = Vector3::dot(thisBox.axis[i], pointOnLine - thisBox.position);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), thisBox.halfSize[i]);
		tmpBoxPos += thisBox.axis[i] * length * mult;
	}
	return Vector3::distance(pointOnLine, tmpBoxPos) <= collider->Radius();
}
