#include "Framework.h"

Grid::Grid() : width(MAX_VALUE), height(MAX_VALUE) {
	material = new Material(L"Shaders/Basic/Grid.hlsl");

	//Axis X
	vertices.emplace_back(-MAX_VALUE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(+MAX_VALUE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	//Axis Y
	vertices.emplace_back(0.0f, -MAX_VALUE, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices.emplace_back(0.0f, +MAX_VALUE, 0.0f, 0.0f, 1.0f, 0.0f);
	//Axis Z
	vertices.emplace_back(0.0f, 0.0f, -MAX_VALUE, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(0.0f, 0.0f, +MAX_VALUE, 0.0f, 0.0f, 1.0f);

	int halfW = width / 2;
	int halfH = height / 2;

	for (int z = -halfH; z <= halfH; z++) {
		if (z == 0) continue;
		vertices.emplace_back(-halfH, 0.0f, z, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(+halfH, 0.0f, z, 0.5f, 0.5f, 0.5f);
	}
	for (int x = -halfH; x <= halfH; x++) {
		if (x == 0) continue;
		vertices.emplace_back(x, 0.0f, -halfH, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(x, 0.0f, +halfH, 0.5f, 0.5f, 0.5f);
	}

	vBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), static_cast<UINT>(vertices.size()));
	wBuffer = new MatrixBuffer();
}

Grid::~Grid() {
	SAFE_DELETE(material);
	SAFE_DELETE(vBuffer );
	SAFE_DELETE(wBuffer );
}

void Grid::Render() {
	wBuffer->SetVSBuffer(0);
	vBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	material->Set();
	DC->Draw(static_cast<UINT>(vertices.size()), 0);
}
