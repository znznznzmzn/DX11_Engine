#include "Framework.h"

SphereCollider::SphereCollider() {
	type = TYPE::SPHERE;
	UINT stackCount = 8;
	UINT sliceCount = 16;
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;
	//Vertices
	for (UINT i = 0; i <= stackCount; i++) {
		float phi = i * phiStep;
		for (UINT j = 0; j <= sliceCount; j++) {
			float theta = j * thetaStep;
			Vertex vertex;
			vertex.pos.x = sin(phi) * cos(theta);
			vertex.pos.y = cos(phi);
			vertex.pos.z = sin(phi) * sin(theta);
			vertices.push_back(vertex);
		}
	}
	//Indices
	for (UINT i = 0; i < stackCount; i++)  {
		for (UINT j = 0; j < sliceCount; j++) {
			indices.push_back((sliceCount + 1) * i + j);     //0
			indices.push_back((sliceCount + 1) * i + j + 1); //1 

			indices.push_back((sliceCount + 1) * i + j);       //0
			indices.push_back((sliceCount + 1) * (i + 1) + j); //2            
		}
	}
	CreateMesh();
}

bool SphereCollider::RayCollision(IN Ray ray, OUT Contact* contact) {
	UpdateWorld();
	Vector3 P = ray.position;
	Vector3 D = ray.direction;
	Vector3 A = P - GlobalPosition();
	float b = Vector3::dot(D, A);
	float c = Vector3::dot(A, A) - Radius() * Radius();
	if (b * b >= c) {
		if (contact != nullptr) {
			float t = -b - sqrt(b * b - c);
			contact->distance = t;
			contact->hitPoint = P + D * t;
		}
		return true;
	}
	return false;
}
bool SphereCollider::BoxCollision(BoxCollider* collider) { return collider->SphereCollision(this); }
bool SphereCollider::SphereCollision(SphereCollider* collider) 
{ return Vector3::distance(GlobalPosition(), collider->GlobalPosition()) <= this->Radius() + collider->Radius(); }
bool SphereCollider::CapsuleCollision(CapsuleCollider* collider) { return collider->SphereCollision(this); }
