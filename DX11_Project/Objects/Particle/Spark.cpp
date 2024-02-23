#include "Framework.h"

void Spark::Create() {
	vertices = new VertexInput[MAX_COUNT];
	vBuffer = new VertexBuffer(vertices, sizeof(VertexInput), MAX_COUNT);
}
void Spark::UpdateParticle() {
	for (UINT i = 0; i < particleCount; i++) {
		vertices[i].pos = Position();
		
		float size = Random::Range(minSize, maxSize);
		vertices[i].size = float2(size, size);
		
		float radius = Random::Range(minRadius, maxRadius);
		Vector3 velocity = Vector3(0, 0, radius);

		Vector3 rot = Vector3(Random::PI(), Random::PI(), Random::PI());

		Matrix rotMat = XMMatrixRotationRollPitchYawFromVector(rot);
		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity, rotMat));
	}
}

Spark::Spark() {
	material->SetShader(L"Shaders/Geometry/Spark.hlsl");
	material->SetDiffuseMap(L"Textures/FX/star.png");
	gShader = Shader::AddGS(L"Shaders/Geometry/Spark.hlsl");

	sparkBuffer = new SparkBuffer();
	startColorBuffer = new ColorBuffer();
	endColorBuffer   = new ColorBuffer();

	blendState[1]->Additive();
	Create();
}
Spark::~Spark() {
	SAFE_DELETE(sparkBuffer);
	SAFE_DELETE(startColorBuffer);
	SAFE_DELETE(endColorBuffer);
	delete[] vertices;
}

void Spark::Update() {
	if (!Active()) return;
	sparkBuffer->data.time += DELTA;
	if (sparkBuffer->data.time > sparkBuffer->data.duration) Stop();
}
void Spark::Render() {
	sparkBuffer->SetVSBuffer(10);
	startColorBuffer->SetPSBuffer(10);
	endColorBuffer->SetPSBuffer(11);
	Particle::Render();
}

void Spark::GUIRender() {
	ImGui::DragFloat3("Direction", (float*)&sparkBuffer->data.direction);
	ImGui::DragFloat ("Duration" , &sparkBuffer->data.duration, 0.10f, 0.0f);
	ImGui::SliderInt("Count", (int*)&particleCount, 0, MAX_COUNT);
	ImGui::DragFloat("MinRadius", &minRadius);
	ImGui::DragFloat("MaxRadius", &maxRadius);
	ImGui::DragFloat("MinSize", &minSize);
	ImGui::DragFloat("MaxSize", &maxSize);

	ImGui::ColorEdit4("StartColor", (float*)&startColorBuffer->data.color);
	ImGui::ColorEdit4("EndColor"  , (float*)&endColorBuffer  ->data.color);

	if (ImGui::Button("Play")) Play(Position());
}

void Spark::Play(Vector3 pos) {
	sparkBuffer->data.time = 0.0f;
	Particle::Play(pos);
	UpdateParticle();
	vBuffer->Update(vertices, particleCount);
}
