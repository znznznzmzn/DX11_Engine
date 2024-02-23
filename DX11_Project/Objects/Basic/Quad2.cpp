#include "Framework.h"

Quad2::Quad2() {
	// front
	vertices.emplace_back(VertexType(-0.5f, -0.5f, 0.0f,  1, 1,  0, 0, -1));
	vertices.emplace_back(VertexType(+0.5f, -0.5f, 0.0f,  0, 1,  0, 0, -1));
	vertices.emplace_back(VertexType(-0.5f, +0.5f, 0.0f,  1, 0,  0, 0, -1));
	vertices.emplace_back(VertexType(+0.5f, +0.5f, 0.0f,  0, 0,  0, 0, -1));

	indices = { 
		0, 2, 1, 1, 2, 3, 
		0, 1, 2, 2, 1, 3 };

	//Calc::Vertex::Normal(vertices, indices);
	Calc::Vertex::Tangent(vertices, indices);

	tag = "Quad2";
	init();
}
