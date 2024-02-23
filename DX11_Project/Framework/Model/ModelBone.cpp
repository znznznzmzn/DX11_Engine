#include "Framework.h"

void ModelBone::SetBones() {
    vector<NodeData> nodes = reader->GetNodes();
    nodeTransforms = new Matrix[nodes.size()];
    UINT nodeIndex = 0;
    for (NodeData node : nodes) {
        Matrix parent;
        int parentIndex = node.parent;
        parent = (parentIndex < 0) ? XMMatrixIdentity() : nodeTransforms[parentIndex];
        nodeTransforms[nodeIndex] = node.transform * parent;
        int boneIndex = reader->GetBone(node.name);
        if (boneIndex >= 0) {
            Matrix offset = reader->GetBones()[boneIndex].offset * nodeTransforms[nodeIndex];
            boneBuffer->SetBone(offset, boneIndex);
        }
        nodeIndex++;
    }
}

ModelBone::ModelBone(string name) : Model(name) {
	boneBuffer = new BoneBuffer();
	SetBones();
	reader->SetShader(L"Shaders/Model/ModelBone.hlsl");
}

ModelBone::~ModelBone() {
    SAFE_DELETE(boneBuffer);
	delete[] nodeTransforms;
}

void ModelBone::Render() {
	boneBuffer->SetVSBuffer(10);
	Model::Render();
}
