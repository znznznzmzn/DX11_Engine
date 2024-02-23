#include "Framework.h"

void Rain::Create() {
	vertices = new VertexSize[MAX_COUNT];
	for (UINT i = 0; i < particleCount; i++) {
		vertices[i].pos.x = Random::Range(-rainBuffer->data.size.x, +rainBuffer->data.size.x);
		vertices[i].pos.y = Random::Range(-rainBuffer->data.size.y, +rainBuffer->data.size.y);
		vertices[i].pos.z = Random::Range(-rainBuffer->data.size.z, +rainBuffer->data.size.z);

		vertices[i].size.x = Random::Range(0.1f, 0.4f);
		vertices[i].size.y = Random::Range(4.0f, 8.0f);
	}

	vBuffer = new VertexBuffer(vertices, sizeof(VertexSize), MAX_COUNT);
}

Rain::Rain() {
	material->SetShader(L"Shaders/Geometry/Rain.hlsl");
	material->SetDiffuseMap(L"Textures/FX/Rain.png");

	gShader = Shader::AddGS(L"Shaders/Geometry/Rain.hlsl");

	rainBuffer = new RainBuffer();
	Create();
}
Rain::~Rain() {
	SAFE_DELETE(rainBuffer);
	delete[] vertices;
}

void Rain::Update() { rainBuffer->data.time += DELTA; }

void Rain::Render() {
	rainBuffer->SetVSBuffer(10);
	Particle::Render();
}

void Rain::GUIRender() {
	ImGui::DragFloat3("Velocity", (float*)&rainBuffer->data.velocity);
	ImGui::DragFloat("Distance", &rainBuffer->data.distance);
	ImGui::DragFloat3("Origin", (float*)&rainBuffer->data.origin);
	ImGui::DragFloat3("Range", (float*)&rainBuffer->data.size);
	ImGui::DragFloat("MinSize", &minSize, 0.1f);
	ImGui::DragFloat("MaxSize", &maxSize, 0.1f);

	ImGui::ColorEdit4("Color", (float*)&rainBuffer->data.rainColor);
}
