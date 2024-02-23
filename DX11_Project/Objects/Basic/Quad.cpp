#include "Framework.h"

Quad::Quad() {
	// front
	vertices.emplace_back(VertexType(-0.5f, -0.5f, 0.0f, 1, 1));
	vertices.emplace_back(VertexType(+0.5f, -0.5f, 0.0f, 0, 1));
	vertices.emplace_back(VertexType(-0.5f, +0.5f, 0.0f, 1, 0));
	vertices.emplace_back(VertexType(+0.5f, +0.5f, 0.0f, 0, 0));

	indices = { 0, 2, 1, 1, 2, 3 };

	Calc::Vertex::Normal(vertices, indices);
	Calc::Vertex::Tangent(vertices, indices);

	tag = "Quad";
	init();
}
