#include "Framework.h"

Sphere_Reversed::Sphere_Reversed(int slice, int stack) {
	tag = "ReversedSphere";
	createMesh(slice, stack);
	//Indices
	for (int i = 0; i < stack; i++) {
		for (int j = 0; j < slice; j++) {
			indices.push_back((slice + 1) * i + j);			 //0
			indices.push_back((slice + 1) * (i + 1) + j);	 //2
			indices.push_back((slice + 1) * i + j + 1);		 //1

			indices.push_back((slice + 1) * (i + 1) + j);	 //2
			indices.push_back((slice + 1) * (i + 1) + j + 1);//3
			indices.push_back((slice + 1) * i + j + 1);		 //1
		}
	}
	Calc::Vertex::Tangent(vertices, indices);
	init();
}