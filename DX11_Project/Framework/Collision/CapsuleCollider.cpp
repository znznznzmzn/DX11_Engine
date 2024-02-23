#include "Framework.h"

CapsuleCollider::CapsuleCollider(float height) {
	type = TYPE::CAPSULE;
    
    UINT stackCount = 9;
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
			vertex.pos.y += height / ((vertex.pos.y > 0) ? 2.0f : -2.0f);
			vertices.push_back(vertex);
		}
	}
	//Indices
	for (UINT i = 0; i < stackCount; i++)  {
		for (UINT j = 0; j < sliceCount; j++) {
			indices.push_back((sliceCount + 1) * i + j);//0
			indices.push_back((sliceCount + 1) * i + j + 1);//1 
			indices.push_back((sliceCount + 1) * i + j);//0
			indices.push_back((sliceCount + 1) * (i + 1) + j);//2            
		}
	}
	CreateMesh();
}

CapsuleCollider::~CapsuleCollider() {
}

bool CapsuleCollider::RayCollision(IN Ray ray, OUT Contact* contact) {
    UpdateWorld();
    Vector3 direction = Up();
    Vector3 pa = GlobalPosition() - direction * Height() * 0.5f;
    Vector3 pb = GlobalPosition() + direction * Height() * 0.5f;

    float r = Radius();

    Vector3 ro = ray.position;
    Vector3 rd = ray.direction;

    Vector3 ba = pb - pa;
    Vector3 oa = ro - pa;

    float baba = Vector3::dot(ba, ba);
    float bard = Vector3::dot(ba, rd);
    float baoa = Vector3::dot(ba, oa);
    float rdoa = Vector3::dot(rd, oa);
    float oaoa = Vector3::dot(oa, oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;
    float h = b * b - a * c;

    if (h >= 0.0f) {
        float t = (-b - sqrt(h)) / a;
        float y = baoa + t * bard;
        if (y > 0.0f && y < baba) {
            if (contact) {
                contact->distance = t;
                contact->hitPoint = ray.position + ray.direction * t;
            }
            return true;
        }

        Vector3 oc = (y <= 0.0f) ? oa : ro - pb;
        b = Vector3::dot(rd, oc);
        c = Vector3::dot(oc, oc) - r * r;
        h = b * b - c;

        if (h > 0.0f) {
            if (contact) {
                contact->distance = t;
                contact->hitPoint = ray.position + ray.direction * t;
            }
            return true;
        }
    }

    return false;
}
bool CapsuleCollider::BoxCollision(BoxCollider* collider) { return collider->CapsuleCollision(this); }
bool CapsuleCollider::SphereCollision(SphereCollider* collider) {
    Vector3 upVector = Up();
    Vector3 A = GlobalPosition() - upVector * Height() * 0.5f;
    Vector3 B = GlobalPosition() + upVector * Height() * 0.5f;
    Vector3 P = collider->GlobalPosition();
    Vector3 pointOnLine = Calc::ClosestPointOnLine(A, B, P);
    return Vector3::distance(P, pointOnLine) <= (Radius() + collider->Radius());
}
bool CapsuleCollider::CapsuleCollision(CapsuleCollider* collider) {
    Vector3 aUpVector = Up();
    Vector3 aA = GlobalPosition() - aUpVector * Height() / 2.0f;
    Vector3 aB = GlobalPosition() + aUpVector * Height() / 2.0f;

    Vector3 bUpVector = collider->Up();
    Vector3 bA = collider->GlobalPosition() - bUpVector * collider->Height() / 2.0f;
    Vector3 bB = collider->GlobalPosition() + bUpVector * collider->Height() / 2.0f;

    Vector3 v0 = bA - aA;
    Vector3 v1 = bB - aA;
    Vector3 v2 = bA - aB;
    Vector3 v3 = bB - aB;

    float d0 = Vector3::dot(v0, v0);
    float d1 = Vector3::dot(v1, v1);
    float d2 = Vector3::dot(v2, v2);
    float d3 = Vector3::dot(v3, v3);

    Vector3 bestA = (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1) ? aB : aA;
    Vector3 bestB = Calc::ClosestPointOnLine(bA, bB, bestA);
    bestA = Calc::ClosestPointOnLine(aA, aB, bestB);

    return Vector3::distance(bestA, bestB) <= (Radius() + collider->Radius());
}
