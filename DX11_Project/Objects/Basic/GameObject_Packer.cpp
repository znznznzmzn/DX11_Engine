#include "Framework.h"

GameObject_Packer::GameObject_Packer() {
	tag = "Pack";
}

GameObject_Packer::~GameObject_Packer() { ClearPack(); }

void GameObject_Packer::ClearPack() {
	for (GameObject*& elem : packs) { 
		elem->NullParent();
		SAFE_DELETE(elem); 
	}
	packs.clear();
}

void GameObject_Packer::Pack() {
	if (packs.empty()) return;
	vertices.clear();
	indices.clear();

	for (GameObject*& elem : packs) {
		Transform* node = new Transform();// 글로벌 위치를 받아올 노드
		node->SetParent(elem);
		for (const VertexType& data : elem->vertices) {
			VertexType vt = data;
			node->Position() = data.pos; // 노드를 버텍스의 위치로 이동
			elem->UpdateWorld();
			node->UpdateWorld(); // GlobalPosition 계산
			vt.pos = node->GlobalPosition(); // vt를 버텍스의 정확한 위치에 배치
			vertices.emplace_back(vt);
		}

		UINT next = (indices.empty())? 0 : indices.back() + 1; // 다음 인덱스 계산
		for (const UINT& idc : elem->indices) indices.emplace_back(next + idc);

		node->NullParent();
		SAFE_DELETE(node);
	}
	Calc::Vertex::Clear_Tangent(vertices);
	Calc::Vertex::Tangent(vertices, indices);
	init();
}
