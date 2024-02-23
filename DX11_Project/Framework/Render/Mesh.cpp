#include "Framework.h"

Mesh::Mesh(void* vertexData, UINT vertexStride, UINT vertexCount, 
	void* indexData, UINT indexCount) {
	vBuffer = new VertexBuffer(vertexData, vertexStride, vertexCount);
	iBuffer = new IndexBuffer((UINT*)indexData, indexCount);
}

Mesh::~Mesh() {
	SAFE_DELETE(vBuffer);
	SAFE_DELETE(iBuffer);
}

void Mesh::IASet(D3D11_PRIMITIVE_TOPOLOGY type) {
	vBuffer->IASet(type);
	iBuffer->IASet();
}

void Mesh::UpdateVertex(void* data, UINT count) { vBuffer->Update(data, count); }
