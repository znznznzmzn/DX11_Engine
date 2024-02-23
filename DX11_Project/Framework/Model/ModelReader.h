#pragma once

class ModelReader {
private:
	string name;
	vector<Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData> nodes;
	vector<BoneData> bones;

	map<string, UINT> boneMap;

	void EjectModel();

	void ReadMaterial();
	void ReadMesh();
public:
	ModelReader(string name);
	~ModelReader();

	void ReadModel(string name);
	void SetShader(wstring file);

	void Render();
	void InstancingRender(const UINT& instanceCount);
	void GUIRender();

	vector<NodeData>& GetNodes() { return nodes; }
	vector<BoneData>& GetBones() { return bones; }
	int GetBone(string name);

	vector<Material*>& GetMaterials() { return materials; }
};