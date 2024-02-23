#pragma once

class GameObject : public Transform {
protected:
	friend class GameObject_Packer;
	typedef VertexUVNormalTangent VertexType;
	vector<VertexType> vertices;
	vector<UINT> indices;

	Material* material;
	Mesh* mesh;
	MatrixBuffer * wBuffer;
	virtual void init(wstring shader_path = L"Shaders/Light/NormalMapping.hlsl");
public:
	~GameObject();
	virtual void Render();
	virtual void InstancingRender(UINT instanceCount);

	vector<VertexType>&	GetVertices() { return vertices; }
	vector<UINT>&		GetIndices () { return indices;  }
	Material*&			GetMaterial() { return material; }
	void SetMaterial(string file) { material->LoadMaterial(file); };
};