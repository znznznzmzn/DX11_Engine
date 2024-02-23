#pragma once

class ModelExpoter {
private:
	Assimp::Importer* importer;
	const aiScene* scene;

	string name;

	vector<Material*> materials;
	vector<MeshData*> meshes;
	vector<NodeData*> nodes;
	vector<BoneData*> bones;

	map<string, UINT> boneMap;
	UINT boneCount = 0;

	//cMaterial
	void ReadMaterial();
	void WriteMaterial();
	string CreateTexture(string file);

	//Mesh
	void ReadMesh(aiNode* node);
	void ReadNode(aiNode* node, int index, int parent);
	void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);
	void WriteMesh();

	//Clip
	Clip* ReadClip(aiAnimation* animation);
	void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);
	void WriteClip(Clip* clip, string clipName, UINT index);

public:
	ModelExpoter(string name, string file);
	~ModelExpoter();

	void ExportMaterial();
	void ExportMesh();
	void ExportClip(string clipName);
};