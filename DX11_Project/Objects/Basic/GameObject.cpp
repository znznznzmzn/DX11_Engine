#include "Framework.h"
#include "GameObject.h"

void GameObject::init(wstring shader_path) {
	material = new Material(shader_path);
	wBuffer = new MatrixBuffer();
	world = XMMatrixIdentity();
	mesh = new Mesh(
		vertices.data(), sizeof(VertexType), static_cast<UINT>(vertices.size()),
		indices.data(), static_cast<UINT>(indices.size()));
}
GameObject::~GameObject() {
	SAFE_DELETE(material);
	SAFE_DELETE(mesh);
	SAFE_DELETE(wBuffer);
}

void GameObject::Render() {
	if (!Active()) return;
	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	mesh->IASet();
	material->Set();
	DC->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}

void GameObject::InstancingRender(UINT instanceCount) {
	if (!Active()) return;
	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	mesh->IASet();
	material->Set();
	DC->DrawIndexedInstanced(indices.size(), instanceCount, 0, 0, 0);
}
