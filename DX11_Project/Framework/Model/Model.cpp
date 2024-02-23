#include "Framework.h"

Model::Model(string name) : name(name) {
	tag = name;
	reader = new ModelReader(name);
	reader->SetShader(L"Shaders/Light/NormalMapping.hlsl");

	wBuffer = new MatrixBuffer();
}

Model::~Model() {
	SAFE_DELETE(reader );
	SAFE_DELETE(wBuffer);
}

void Model::Render() {
	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	reader->Render();
}

void Model::InstancingRender(const UINT& instanceCount) {
	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	reader->InstancingRender(instanceCount);
}

void Model::GUIRender() {
	Transform::GUIRender();
	reader->GUIRender();
}
