#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

struct MeshData {
	string name;
	UINT mererialIndex;
	vector<ModelVertex> vertices;
	vector<UINT> indices;
};
struct NodeData {
	int index;
	string name;
	int parent;
	Matrix transform;
};
struct BoneData {
	string name;
	int index;
	Matrix offset;
};

struct VertexWeights {
	UINT  indices[4] = {}; // Bone Index
	float weights[4] = {}; // Bone Interpolation
	void Add(const UINT& index, const float& weight) {
		for (UINT i = 0; i < 4; i++) {
			if (weights[i] == 0.0f) {
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}
	void Normalize() {
		float sum = weights[0] + weights[1] + weights[2] + weights[3];
		weights[0] /= sum;
		weights[1] /= sum;
		weights[2] /= sum;
		weights[3] /= sum;
	}
};

struct KeyTransform {
	float3 scale;
	float4 rotation;
	float3 position;
};

struct KeyFrame {
	string boneName;
	vector<KeyTransform> transforms;
};
struct ClipNode {
	aiString name;
	vector<KeyTransform> transforms;
};
struct Clip {
	string name;
	UINT framecount;
	float tickPerSecond;
	vector<KeyFrame*> keyFrames;
};

struct ClipTransform {
	//Matrix transform[FRAME][BONE]
	vector<vector<Matrix>> transform;
};

struct InstanceData {
	Matrix world;
	int index = 0;
	InstanceData() { world = XMMatrixIdentity(); }
};