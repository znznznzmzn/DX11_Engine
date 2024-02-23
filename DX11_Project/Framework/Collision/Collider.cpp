#include "Framework.h"

void Collider::CreateMesh() {
    mesh = new Mesh(
        vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
}

Collider::Collider() {
    material = new Material(L"Shaders/Basic/Collider.hlsl");
    material->GetData().diffuse = { 0.0f, 1.0f, 0.0f, 0.8f };
    wBuffer = new MatrixBuffer();
}

Collider::~Collider() {
    SAFE_DELETE(material);
    SAFE_DELETE(mesh);
    SAFE_DELETE(wBuffer);
}

void Collider::Render() {
    if (!Active()) return;

    wBuffer->Set(world);
    wBuffer->SetVSBuffer(0);

    mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}

bool Collider::Collision(Collider* collider) {
    if (!Active() || !collider->Active()) return false;

    UpdateWorld();
    collider->UpdateWorld();

    switch (collider->type) {
    case Collider::TYPE::BOX:     return BoxCollision    ((BoxCollider*    )collider);
    case Collider::TYPE::SPHERE:  return SphereCollision ((SphereCollider* )collider);
    case Collider::TYPE::CAPSULE: return CapsuleCollision((CapsuleCollider*)collider);
    default: break;
    }

    return false;
}
