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
		Transform* node = new Transform();// �۷ι� ��ġ�� �޾ƿ� ���
		node->SetParent(elem);
		for (const VertexType& data : elem->vertices) {
			VertexType vt = data;
			node->Position() = data.pos; // ��带 ���ؽ��� ��ġ�� �̵�
			elem->UpdateWorld();
			node->UpdateWorld(); // GlobalPosition ���
			vt.pos = node->GlobalPosition(); // vt�� ���ؽ��� ��Ȯ�� ��ġ�� ��ġ
			vertices.emplace_back(vt);
		}

		UINT next = (indices.empty())? 0 : indices.back() + 1; // ���� �ε��� ���
		for (const UINT& idc : elem->indices) indices.emplace_back(next + idc);

		node->NullParent();
		SAFE_DELETE(node);
	}
	Calc::Vertex::Clear_Tangent(vertices);
	Calc::Vertex::Tangent(vertices, indices);
	init();
}
