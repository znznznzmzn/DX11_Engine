#include "Framework.h"

ModelMesh::ModelMesh(string name) : name(name) {
}

ModelMesh::~ModelMesh() {
	SAFE_DELETE(mesh);

	delete[] vertices;
	delete[] indices;
}

void ModelMesh::Render() {
	mesh->IASet();
	material->Set();
	DC->DrawIndexed(indexCount, 0, 0);
}
void ModelMesh::InstancingRender(const UINT& instanceCount) {
	mesh->IASet();
	material->Set();
	DC->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

void ModelMesh::CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount) {
	mesh = new Mesh(vertexData, sizeof(ModelVertex), vertexCount,
		indexData, indexCount);
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;

	vertices = (ModelVertex*)vertexData;
	indices = (UINT*)indexData;
}
