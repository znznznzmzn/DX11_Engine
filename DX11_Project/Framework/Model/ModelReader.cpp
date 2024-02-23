#include "Framework.h"

ModelReader::ModelReader(string name) : name(name) {
	ReadModel(name);
}

ModelReader::~ModelReader() {
	EjectModel();
}

void ModelReader::ReadModel(string name) {
	ReadMaterial();
	ReadMesh();
}

void ModelReader::SetShader(wstring file) 
{ for (Material*& elem : materials) elem->SetShader(file); }

void ModelReader::Render() 
{ for (ModelMesh*& elem : meshes) elem->Render(); }
void ModelReader::InstancingRender(const UINT& instanceCount) 
{ for (ModelMesh*& elem : meshes) elem->InstancingRender(instanceCount); }

void ModelReader::GUIRender() 
{ for (Material*& elem : materials) elem->GUIRender(); }

int ModelReader::GetBone(string name) {
	if (boneMap.count(name) == 0) return -1;
	return boneMap[name];
}

void ModelReader::EjectModel() {
	for (Material * elem : materials) { SAFE_DELETE(elem); }
	for (ModelMesh* elem : meshes   ) { SAFE_DELETE(elem); }
	materials.clear();
	meshes.clear();
}

void ModelReader::ReadMaterial() {
	string path = "Models/Materials/" + name + "/" + name + ".mats";
	BinaryReader* r = new BinaryReader(path);

	if (r->IsFailed()) assert(false);

	UINT size = r->UInt();

	materials.resize(size);

	for (Material*& elem : materials) {
		elem = new Material;
		elem->LoadMaterial(r->String());
	}

	delete r;
}

void ModelReader::ReadMesh() {
	string path = "Models/Meshes/" + name + ".mesh";
	BinaryReader* r = new BinaryReader(path);

	UINT size = r->UInt();

	meshes.resize(size);
	for (ModelMesh*& elem : meshes) {
		elem = new ModelMesh(r->String());
		UINT materialIndex = r->UInt();
		elem->SetMaterial(materials[materialIndex]);

		UINT vertexCount = r->UInt();
		ModelVertex* vertices = new ModelVertex[vertexCount];
		r->Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

		UINT indexCount = r->UInt();
		UINT* indices = new UINT[indexCount];
		r->Byte((void**)&indices, sizeof(UINT) * indexCount);

		elem->CreateMesh(vertices, vertexCount, indices, indexCount);
	}

	size = r->UInt();
	nodes.resize(size);
	for (NodeData& elem : nodes) {
		elem.index = r->Int();
		elem.name = r->String();
		elem.parent = r->Int();
		elem.transform = r->Matrix();
	}

	size = r->UInt();
	bones.resize(size);
	for (BoneData& elem : bones) {
		elem.name = r->String();
		elem.index = r->Int();
		elem.offset = r->Matrix();

		boneMap[elem.name] = elem.index;
	}

	delete r;
}

