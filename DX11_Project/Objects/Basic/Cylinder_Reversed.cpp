#include "Framework.h"

void Cylinder_Reversed::setVertex(int slice) {
	if (slice < 3) slice = 3;
	const float height = 0.5f;
	vertices.clear();
	indices.clear();

#pragma region Vertices
	vertices.emplace_back(VertexType(0.0f, +0.5f, 0.0f, 0.5f, 0.5f));
	for (int i = 0; i <= slice; i++) {
		float degree = 360.0f * (i / (float)slice);
		VertexType elem;
		elem.pos.x = cos(degree * Calc::Degree_to_PI) / 2;
		elem.pos.y = height;
		elem.pos.z = sin(degree * Calc::Degree_to_PI) / 2;

		elem.uv.x = i / (float)slice;
		elem.uv.y = 0;

		elem.normal = { 0.0f, 0.0f, 0.0f };

		vertices.emplace_back(elem);
	}
	for (int i = 0; i <= slice; i++) {
		float degree = 360.0f * (i / (float)slice);
		VertexType elem;
		elem.pos.x = cos(degree * Calc::Degree_to_PI) / 2;
		elem.pos.y = -height;
		elem.pos.z = sin(degree * Calc::Degree_to_PI) / 2;

		elem.uv.x = i / (float)slice;
		elem.uv.y = 1;

		elem.normal = { 0.0f, 0.0f, 0.0f };

		vertices.emplace_back(elem);
	}
	vertices.emplace_back(VertexType(0.0f, -0.5f, 0.0f, 0.5f, 0.5f));
#pragma endregion

#pragma region Indices
	for (int i = 0; i < slice; i++) { // ���� �ε���
		indices.emplace_back(0);
		indices.emplace_back(i + 1);
		indices.emplace_back(i + 2);
	}
	for (int i = 1; i <= slice; i++) { // ���� �ε���
		indices.emplace_back(i);
		indices.emplace_back(i + slice + 1);
		indices.emplace_back(i + 1);
		indices.emplace_back(i + slice + 1);
		indices.emplace_back(i + slice + 2);
		indices.emplace_back(i + 1);
	}
	for (int i = slice + 2; i < slice + slice + 2; i++) { // �Ʒ��� �ε���
		indices.emplace_back(slice + slice + 3);
		indices.emplace_back(i + 1);
		indices.emplace_back(i);
	}
#pragma endregion

	Calc::Vertex::Normal(vertices, indices);
	Calc::Vertex::Tangent(vertices, indices);
}

Cylinder_Reversed::Cylinder_Reversed(int slice) {
	tag = "Cylinder_Reversed";
	setVertex(slice);
	init();
}
