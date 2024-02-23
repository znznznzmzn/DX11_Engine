#include "Framework.h"

GUI_Basic::GUI_Basic(wstring image_path) {
	tag = "GUI_Basic";
	SetTexture(image_path);
	init(L"Shaders/Basic/Texture.hlsl");
}
GUI_Basic::~GUI_Basic() {
	SAFE_DELETE(mesh   );
	SAFE_DELETE(wBuffer);
}

void GUI_Basic::init(wstring shader_path) {
	SetShader(shader_path);

	vertices.emplace_back(VertexType(-0.5f, -0.5f, 0.0f, 0, 1));
	vertices.emplace_back(VertexType(-0.5f, +0.5f, 0.0f, 0, 0));
	vertices.emplace_back(VertexType(+0.5f, +0.5f, 0.0f, 1, 0));
	vertices.emplace_back(VertexType(+0.5f, -0.5f, 0.0f, 1, 1));
	indices = { 0, 1, 2, 2, 3, 0 };

	wBuffer = new MatrixBuffer();
	cBuffer = new ColorBuffer();
	world = XMMatrixIdentity();
	mesh = new Mesh(
		vertices.data(), sizeof(VertexType), static_cast<UINT>(vertices.size()),
		indices.data(), static_cast<UINT>(indices.size()));
}

void GUI_Basic::SetTexture(wstring texture_path) {
	texture = Texture::Add(texture_path);
	if (texture != nullptr) {
		Scale().x = texture->Width();
		Scale().y = texture->Height();
	}
}

void GUI_Basic::SetShader(wstring file) {
	vShader = Shader::AddVS(file);
	pShader = Shader::AddPS(file);
}

void GUI_Basic::PostRender() {
	if (!Active()) return;
	wBuffer->Set(world); 
	wBuffer->SetVSBuffer(0);
	cBuffer->SetPSBuffer(4);
	mesh->IASet();
	if (texture != nullptr) texture->PSSet(0);
	vShader->Set();
	pShader->Set();
	DC->DrawIndexed(6, 0, 0);
}

void GUI_Basic::GUIRender() {
	ImGui::ColorEdit4("Color", (float*)&cBuffer->data.color);
	if (EditorGUI::ImageButton_Dialog_LoadTexture("Texture", "LoadTexture", texture))
		SetTexture(texture->GetPath());
	ImGui::SameLine();
	if (ImGui::Button("X", { 24, 24 })) SetTexture(L"");
}


