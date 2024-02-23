#include "Framework.h"

ModelInstancing::ModelInstancing(string name, UINT max_count) : name(name) {
    instanceDatas.resize(max_count);
    instanceBuffer = new VertexBuffer(instanceDatas.data(), sizeof(InstanceData), instanceDatas.size());
    reader = new ModelReader(name);
    reader->SetShader(L"Shaders/Instancing/ModelInstancing.hlsl");
}
ModelInstancing::~ModelInstancing() {
    for (Transform* elem : transforms) { SAFE_DELETE(elem); }
}

void ModelInstancing::Update() {
    drawCount = 0;
    for (UINT i = 0; i < transforms.size(); i++) {
        if (transforms[i]->Active()) {
            transforms[i]->UpdateWorld();
            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
            instanceDatas[drawCount].index = i;
            drawCount++;
        }
    }
    instanceBuffer->Update(instanceDatas.data(), drawCount);
}
void ModelInstancing::Render() {
    instanceBuffer->IASet(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);
    reader->InstancingRender(drawCount);
}
void ModelInstancing::GUIRender() {
    ImGui::Text("DrawCount : %d", drawCount);
    for (Transform* elem : transforms) elem->GUIRender();
}

Transform* ModelInstancing::Add() {
    Transform* newElem = new Transform();
    newElem->Tag() = name + "_" + to_string(transforms.size());
    transforms.push_back(newElem);
    return newElem;
}
